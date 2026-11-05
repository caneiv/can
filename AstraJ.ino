#include "GyverButton.h"
#include <SPI.h>
#include "mcp_can.h"
#define PIN_RELAY 7 //D7 вързано към релето
#define BTN_PIN 3   // Бутона е включен така D3 (BTN_PIN --- БУТОН --- GND) 
// HIGH_PULL - бутона е включен към GND, пин е свързан VCC (BTN_PIN --- БУТОН --- GND)
GButton butt1(BTN_PIN); //(по подразбиране HIGH_PULL и NORM_OPEN)

// Инициализация на CAN interface
MCP_CAN CAN(10);  //CAN CS pin

void CANSetup(void)
{
 Serial.begin(115200);
  // инициализиране на скоростта: 33k3
  if (CAN.begin(CAN_33KBPS, MCP_8MHz)==CAN_OK) Serial.print("CAN init ok\r\n");
  else Serial.print("CAN init failed\r\n");

  while (!Serial);
  Serial.begin(115200);
  SPI.begin();
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_RELAY, OUTPUT);    // обевяваме D7 като изход
  digitalWrite(PIN_RELAY, HIGH); // Изключваме релето - изпращаме високо ниво
  butt1.setDebounce(50);         // настройка антидребезга (по подразбиране 80 мс)
  butt1.setTimeout(300);         // настройка таймаут на задържане (по подразбиране 500 мс)
  butt1.setClickTimeout(600);    // настройка таймаут между натискането (по подразбиране 300 мс)
  
}
void loop() {
  butt1.tick();  // Задължителна функция за обработка. Должна постоянно опрашиваться
  if (butt1.isSingle()) Serial.println("Single");     // Проверка за единично натискане
   unsigned char canMsg[4] = {0x04, 0x03, 0x00, 0x00};
  // format:  (id, id_type, dlc, data buf)
  CAN.sendMsgBuf(0x10754040, 1, 4, canMsg); // CAN за активиране на видео
  digitalWrite(PIN_RELAY, LOW); // Включваме релето - изпеащаме ниско ниво
  Serial.print("send video on\r\n");
  
  if (butt1.isDouble()) Serial.println("Double");     // Проверка за двойно натискане
 unsigned char canMsg2[4] = {0x04, 0x00, 0x00, 0x00};
  // format:  (id, id_type, dlc, data buf)
  CAN.sendMsgBuf(0x10754040, 1, 4, canMsg2); // CAN за деактивиране на видео
  digitalWrite(PIN_RELAY, HIGH); // Изключваме релето - изпращаме високо ниво
  Serial.print("send video off\r\n");
  
  if (butt1.isTriple()) Serial.println("Triple");     //Проверка за тройно натискане
}
