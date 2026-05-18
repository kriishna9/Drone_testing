#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Servo.h>

// Hardware Objects
RF24 radio(7, 8);
Adafruit_MPU6050 mpu;
Servo m1, m2, m3, m4;

const byte address[6] = "00001";
int receivedPackets[2] = {100, 0}; // [Throttle, Mode]

// PID aur Angles ke variables
float inputThrottle = 1000;
float errorPitch, errorRoll;
float lastErrorPitch, lastErrorRoll;
float pidOutputPitch, pidOutputRoll;

// PID Gains (Ise bad mein tune karna padega)
float Kp = 1.5;

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    // 1. ESC Attachment
    m1.attach(3);
    m2.attach(5);
    m3.attach(6);
    m4.attach(9);
    m1.writeMicroseconds(1000);
    m2.writeMicroseconds(1000);
    m3.writeMicroseconds(1000);
    m4.writeMicroseconds(1000);

    // 2. Init MPU6050
    if (!mpu.begin())
    {
        Serial.println("MPU Fail!");
        while (1)
            ;
    }

    // 3. Init NRF
    if (!radio.begin())
    {
        Serial.println("NRF Fail!");
        while (1)
            ;
    }
    radio.openReadingPipe(0, address);
    radio.setDataRate(RF24_250KBPS);
    radio.startListening();

    Serial.println("🛸 Flight Controller System Ready!");
}

void loop()
{
    // Step A: Remote Data Catch Karo
    if (radio.available())
    {
        radio.read(&receivedPackets, sizeof(receivedPackets));
        // Remote se aane wali value ko 1000-2000 ms ke throttle mein badlo
        inputThrottle = map(receivedPackets[0], 100, 999, 1000, 1800);
    }

    // Step B: Gyro se Balance Read Karo
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    float currentPitch = g.gyro.y;
    float currentRoll = g.gyro.x;

    // Step C: Basic Proportional (P) Balancing Calculation
    errorPitch = 0 - currentPitch; // 0 target angle hai (flat)
    errorRoll = 0 - currentRoll;

    pidOutputPitch = errorPitch * Kp;
    pidOutputRoll = errorRoll * Kp;

    // Step D: Motors Speed Mixing (Quad-X Configuration)
    if (inputThrottle > 1100)
    {                                                                 // Sirf tab chalega jab throttle thoda upar ho
        int speedM1 = inputThrottle + pidOutputPitch + pidOutputRoll; // Front Left
        int speedM2 = inputThrottle + pidOutputPitch - pidOutputRoll; // Front Right
        int speedM3 = inputThrottle - pidOutputPitch + pidOutputRoll; // Back Left
        int speedM4 = inputThrottle - pidOutputPitch - pidOutputRoll; // Back Right

        // Safety limits constraint (1000 to 2000)
        m1.writeMicroseconds(constrain(speedM1, 1000, 2000));
        m2.writeMicroseconds(constrain(speedM2, 1000, 2000));
        m3.writeMicroseconds(constrain(speedM3, 1000, 2000));
        m4.writeMicroseconds(constrain(speedM4, 1000, 2000));
    }
    else
    {
        // Agar throttle down hai toh sab stop
        m1.writeMicroseconds(1000);
        m2.writeMicroseconds(1000);
        m3.writeMicroseconds(1000);
        m4.writeMicroseconds(1000);
    }
}