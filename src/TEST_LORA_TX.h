#include <SPI.h>
#include <LoRa.h>

// Chân LoRa
#define LORA_SCK   5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_SS   18
#define LORA_RST  14
#define LORA_DIO0 26

// Tần số
#define BAND 433E6      // 433MHz
// #define BAND 868E6
// #define BAND 915E6

unsigned long lastSend = 0;

void setup()
{
    Serial.begin(115200);

    SPI.begin(
        LORA_SCK,
        LORA_MISO,
        LORA_MOSI,
        LORA_SS
    );

    LoRa.setPins(
        LORA_SS,
        LORA_RST,
        LORA_DIO0
    );

    Serial.println("LoRa TX Start");

    if (!LoRa.begin(BAND))
    {
        Serial.println("LoRa Init Failed");
        while (1);
    }

    Serial.println("LoRa Init OK");
}

void loop()
{
    if (millis() - lastSend > 2000)
    {
        lastSend = millis();

        String msg =
            "Voltage=220,Current=1.2";

        LoRa.beginPacket();

        LoRa.print(msg);

        LoRa.endPacket();

        Serial.print("Send: ");
        Serial.println(msg);
    }
}