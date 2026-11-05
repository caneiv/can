#include <HardwareCAN.h>
#include "OneButton.h"

HardwareCAN canBus(CAN1_BASE);
CanMsg msg ;

void CANSetup(void)
{
  CAN_STATUS Stat ;
  canBus.map(CAN_GPIO_PB8_PB9);
  Stat = canBus.begin(CAN_SPEED_95, CAN_MODE_NORMAL);
  }
CAN_TX_MBX CANsend(CanMsg *pmsg) 
{
  CAN_TX_MBX mbx;
  do
  {
    mbx = canBus.send(pmsg) ;
  }
  while (mbx == CAN_TX_NO_MBX) ;     
  return mbx ;
}
void SendCANmessage(long id = 0x001, byte dlength = 8, byte d0 = 0x00, byte d1 = 0x00, byte d2 = 0x00, byte d3 = 0x00, byte d4 = 0x00, byte d5 = 0x00, byte d6 = 0x00, byte d7 = 0x00)
{
  msg.IDE = CAN_ID_STD;          // Indicates a standard identifier ; CAN_ID_EXT would mean this frame uses an extended identifier
  msg.RTR = CAN_RTR_DATA;        // Indicated this is a data frame, as opposed to a remote frame (would then be CAN_RTR_REMOTE)
  msg.ID = id ;                  // Identifier of the frame :
  msg.DLC = dlength;             // Number of data bytes to follow
  msg.Data[0] = d0 ;
  msg.Data[1] = d1 ;
  msg.Data[2] = d2 ;
  msg.Data[3] = d3 ;
  msg.Data[4] = d4 ;
  msg.Data[5] = d5 ;
  msg.Data[6] = d6 ;
  msg.Data[7] = d7 ;

  CANsend(&msg) ;      // Send this frame
  delay(10);
}
OneButton button1(PA0, true);
OneButton button2(PA1, true);

void setup()
{
CANSetup() ;
// бутон 1
  button1.attachClick(click1);
  button1.attachDoubleClick(doubleclick1);
  button1.attachDuringLongPress(longPress1);
  //  бутон 2 
  button2.attachClick(click2);
  button2.attachDoubleClick(doubleclick2);
  button2.attachDuringLongPress(longPress2);
}

void loop() {
  button1.tick();
  button2.tick();
  delay(10);
} 
void click1() {
  //  при едно натискане на бутон 1
  SendCANmessage(0x290, 8, 0x80, 0x7c, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00); //scroll up
} // click1

void doubleclick1() {
   // двойно натискане на бутон 1
   SendCANmessage(0x290, 8, 0x80, 0x7d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //ok
} // doubleclick1

void longPress1() {
 //  дълго натискане на бутон 1
 SendCANmessage(0x290, 8, 0x80, 0xe4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //BC
} // longPress1

// ... функции бутон 2
void click2() {
 //  натискане на бутон 2
 SendCANmessage(0x290, 8, 0x80, 0x7c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00); //scroll down
} // click2


void doubleclick2() {
//   двойно натискане на бутон 2
SendCANmessage(0x290, 8, 0x81, 0x96, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00); //on
} // doubleclick2


void longPress2() {
 // Serial.println("Button 2 longPress..."); дълго натискане на бутон 2
 SendCANmessage(0x290, 8, 0x80, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); // stanby on
 
} // longPress2




