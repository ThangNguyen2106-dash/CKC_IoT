#include <SPI.h>
#include <LoRa.h>

#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_SS 18
#define LORA_RST 14
#define LORA_DIO0 26

#define BAND 433E6

void setup()
{
    Serial.begin(115200);

    SPI.begin(
        LORA_SCK,
        LORA_MISO,
        LORA_MOSI,
        LORA_SS);

    LoRa.setPins(
        LORA_SS,
        LORA_RST,
        LORA_DIO0);

    Serial.println("LoRa RX Start");

    if (!LoRa.begin(BAND))
    {
        Serial.println("LoRa Init Failed");
        while (1)
            ;
    }

    Serial.println("Waiting...");
}

void loop()
{
    int packetSize = LoRa.parsePacket();

    if (packetSize)
    {
        String data = "";

        while (LoRa.available())
        {
            data += (char)LoRa.read();
        }

        Serial.print("Received: ");
        Serial.println(data);

        Serial.print("RSSI: ");
        Serial.println(LoRa.packetRssi());
    }
}