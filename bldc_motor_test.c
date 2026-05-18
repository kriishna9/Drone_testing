#include <Servo.h>

// Charo motors ke liye Servo objects
Servo motor1, motor2, motor3, motor4;

void setup()
{
    Serial.begin(9600);
    Serial.println("--- MOTOR SAFE SPIN TEST ---");

    // ESCs ko pins ke sath attach karein
    motor1.attach(3);
    motor2.attach(5);
    motor3.attach(6);
    motor4.attach(9);

    // ESCs ko arm (activate) karne ke liye 1 second tak ZERO signal bhejenge
    Serial.println("⏳ Arming ESCs... Please wait.");
    motor1.writeMicroseconds(1000);
    motor2.writeMicroseconds(1000);
    motor3.writeMicroseconds(1000);
    motor4.writeMicroseconds(1000);
    delay(2000); // 2 second ka wait ESC beep ke liye

    Serial.println("🚀 Motors Armed! Starting Slow Spin...");
}

void loop()
{
    // 1150 ya 1200 microsecond par motors bilkul dhire se chalna shuru hoti hain
    int testSpeed = 1150;

    Serial.print("Spinning Motors at Safe Speed: ");
    Serial.println(testSpeed);

    motor1.writeMicroseconds(testSpeed);
    motor2.writeMicroseconds(testSpeed);
    motor3.writeMicroseconds(testSpeed);
    motor4.writeMicroseconds(testSpeed);

    delay(5000); // 5 second tak ghoomegi

    // Motors ko band karne ke liye
    Serial.println("🛑 Stopping Motors.");
    motor1.writeMicroseconds(1000);
    motor2.writeMicroseconds(1000);
    motor3.writeMicroseconds(1000);
    motor4.writeMicroseconds(1000);

    while (1)
        ; // Code yahan ruka rahega taaki loop bar-bar na chale
}