#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    Serial.println("--- MPU6050 GYRO TEST ---");

    if (!mpu.begin())
    {
        Serial.println("❌ MPU6050 Sensor Not Found! Check SDA/SCL Wires.");
        while (1)
            ;
    }
    Serial.println("✅ MPU6050 Online!");
    delay(100);
}

void loop()
{
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Serial Monitor ya Serial Plotter par angles dekho
    Serial.print("Rotation X (Roll): ");
    Serial.print(g.gyro.x);
    Serial.print(" | Y (Pitch): ");
    Serial.println(g.gyro.y);

    delay(200); // Har 0.2 second mein data read karega
}