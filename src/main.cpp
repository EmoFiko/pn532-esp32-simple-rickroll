#include <Arduino.h>
#include <PN532_HSU.h>
#include <PN532.h>
#include <emulatetag.h>
#include <NdefMessage.h>

// PN532 HSU on ESP32
// TX -> GPIO 16 (RX2)
// RX -> GPIO 17 (TX2)
// DIP switches: OFF - OFF
PN532_HSU pn532_hsu(Serial2);
PN532 nfc(pn532_hsu);
EmulateTag emu(pn532_hsu);

uint8_t ndefBuf[128];

void setup()
{
  Serial.begin(9600);
  Serial2.setRxBufferSize(1024);
  delay(1000);

  Serial.println("\n--- PN532 Rickroll NDEF Emulator ---");
  Serial.println("Power on -> emulates Rickroll NDEF tag.");

  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata)
  {
    Serial.println("PN532 BULUNAMADI!");
    while (1)
      ;
  }
  Serial.print("PN532 Bulundu. Chip: PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);

  emu.init();

  // Rickroll URI NDEF kaydi
  NdefMessage message = NdefMessage();
  message.addUriRecord("https://www.youtube.com/watch?v=dQw4w9WgXcQ");
  int msgSize = message.getEncodedSize();
  message.encode(ndefBuf);
  emu.setNdefFile(ndefBuf, msgSize);

  // Sabit UID (3 byte)
  uint8_t defaultUid[3] = {0x12, 0x34, 0x56};
  emu.setUid(defaultUid);

  Serial.println("NDEF Rickroll hazir. Telefonu PN532'ye yaklastir.");
}

void loop()
{
  if (emu.emulate(1000))
  {
    Serial.println("Telefon NDEF'i okudu (Rickroll).");
  }

  // Seri porta herhangi bir karakter yazilinca durdur
  if (Serial.available() > 0)
  {
    while (Serial.available())
      Serial.read();
    Serial.println("Durduruldu.");
    while (1)
      ;
  }

  delay(50);
}
