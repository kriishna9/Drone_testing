#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE = D7, CSN = D8
const byte address[6] = "00001";
int receivedData[2];

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    Serial.println("--- NRF RECEIVER TEST ---");

    if (!radio.begin())
    {
        Serial.println("❌ NRF Hardware Error! Check Wires.");
        while (1)
            ;
    }

    radio.openReadingPipe(0, address);
    radio.setDataRate(RF24_250KBPS); // High range setup
    radio.setPALevel(RF24_PA_MAX);
    radio.startListening();
    Serial.println("📡 NRF Online! Waiting for packets...");
}

void loop()
{
    if (radio.available())
    {
        radio.read(&receivedData, sizeof(receivedData));
        Serial.print("🎉 Received Packet: ");
        Serial.println(receivedData[0]);
    }
}