#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
// codigo para o servidor BLE com ultrassonico
BLECharacteristic *characteristicTX1, *characteristicTX2;

bool deviceConnected = false;

#define trigger GPIO_NUM_26
#define echo GPIO_NUM_25


#define Agua  "2aaa9a64-6c81-4718-95eb-92fcdc1f95c8"
#define CHARACTERISTIC_UUID_RX0 "1ec6580b-37ec-4205-b8ee-702c3e159347"
#define CHARACTERISTIC_UUID_TX0 "a140d3d3-4ece-4c37-9a12-37de04b65e4b"

#define Oleo "c4352ec5-7869-462b-b064-9065a9a55800"
#define CHARACTERISTIC_UUID_RX1 "e86e8e83-2db0-4f6f-b2c3-7dfe6f3d4e22"
#define CHARACTERISTIC_UUID_TX1 "17df5d0b-7068-4f0e-93d3-49033485eef5"

class CharacteristicCallbacks: public BLECharacteristicCallbacks {
     void onWrite(BLECharacteristic *characteristic) {
          //retorna ponteiro para o registrador contendo o valor atual da caracteristica
          std::string rxValue = characteristic->getValue();
          //verifica se existe dados (tamanho maior que zero)
          if (rxValue.length() > 0) {
               for (int i = 0; i < rxValue.length(); i++) {
             Serial.print(rxValue[i]);
               }
               Serial.println();
               //L1 liga o LED | L0 desliga o LED
               if (rxValue.find("L1") != -1) { 
             Serial.print("a");
               }
               else if (rxValue.find("L0") != -1) {
             Serial.print("b");
               }
               // B1 liga o Buzzer | B0 desliga o Buzzer
               else if (rxValue.find("B1") != -1) { 
             Serial.print("c");
               }
               else if (rxValue.find("B0") != -1) {
             Serial.print("d");
               }
          }
     }//onWrite
};

//callback para receber os eventos de conexão de dispositivos
class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
 deviceConnected = true;
    };
 
    void onDisconnect(BLEServer* pServer) {
 deviceConnected = false;
    }
};

float tempo,dist,media = 0;

void ultrassonico(gpio_num_t A, gpio_num_t B)

{

  gpio_set_direction(A, GPIO_MODE_OUTPUT);
  gpio_set_direction(B, GPIO_MODE_INPUT);

    }

void sonico(gpio_num_t A, gpio_num_t B)

{

  gpio_set_level(A,0);
  delayMicroseconds(2);
  gpio_set_level(A,1);
  delayMicroseconds(10);
  gpio_set_level(A,0);
  tempo = pulseIn(B,HIGH);

}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ultrassonico(trigger,echo);
  BLEDevice::init("Maquina de Sabao");

  BLEServer *server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());


  BLEService *service1 = server->createService(Agua);
  BLEService *service2 = server->createService(Oleo);

  characteristicTX1 = service1->createCharacteristic(CHARACTERISTIC_UUID_TX0, BLECharacteristic::PROPERTY_NOTIFY);
  characteristicTX2 = service2->createCharacteristic(CHARACTERISTIC_UUID_TX1, BLECharacteristic::PROPERTY_NOTIFY);

  characteristicTX1 ->addDescriptor(new BLE2902());
  characteristicTX2 ->addDescriptor(new BLE2902());

  BLECharacteristic *characteristic1 = service1->createCharacteristic(
                                                      CHARACTERISTIC_UUID_RX0,
                                                      BLECharacteristic::PROPERTY_WRITE
                                                    );

  BLECharacteristic *characteristic2 = service2->createCharacteristic(
                                                      CHARACTERISTIC_UUID_RX1,
                                                      BLECharacteristic::PROPERTY_WRITE
                                                    );
 
    characteristic1->setCallbacks(new CharacteristicCallbacks());
    characteristic2->setCallbacks(new CharacteristicCallbacks());
    
 
    // Start the service
    service1->start();
    service2->start();
 
    // Start advertising (descoberta do ESP32)
    server->getAdvertising()->start();
  

}

void loop() {
  // put your main code here, to run repeatedly:
  if (deviceConnected) {
      sonico(trigger,echo);
    dist= tempo*0.034/2;
    char txString[8];
      for(int i = 0; i<9; i++)
      {
      media += dist/10;
      }
      
  dtostrf(media, 2, 2, txString);
  characteristicTX1->setValue(txString); //seta o valor que a caracteristica notificará (enviar) 
  characteristicTX1->notify(); 

  delay(10);
  tempo = 0;
  media = 0;
    
    }

}
