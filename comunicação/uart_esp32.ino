#include "HardwareSerial.h"
#define RXD2 16
#define TXD2 17

HardwareSerial MySerial(1);
void setup() {

  MySerial.begin(115200, SERIAL_8N1, 16, 17);
  MySerial.write(1);


void loop() {
  
   while (MySerial.available() > 0) {
        int byteFromSerial = MySerial.read();
        // Do something
        if(byteFromSerial == 3)
        {
           MySerial.write(1);
            delay(5000);
           MySerial.write(2); 
            delay(5000);
        }
    //Write something like that
      MySerial.println(byteFromSerial);
    }
   
}
