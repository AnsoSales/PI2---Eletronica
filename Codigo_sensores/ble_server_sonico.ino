#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

BLECharacteristic *characteristicTX0, *characteristicTX1, *characteristicTX2, *characteristicTX3,*characteristicTX4, *characteristicTX5;

bool deviceConnected = false;
float tempo,volume,dist,media,altura,raio = 0.0;
int comando = 0;
#define trigger GPIO_NUM_26
#define echo GPIO_NUM_25


#define Alcool  "2aaa9a64-6c81-4718-95eb-92fcdc1f95c8"
#define CHARACTERISTIC_UUID_TX0 "a140d3d3-4ece-4c37-9a12-37de04b65e4b"

#define Oleo "c4352ec5-7869-462b-b064-9065a9a55800"
#define CHARACTERISTIC_UUID_TX1 "17df5d0b-7068-4f0e-93d3-49033485eef5"

#define Essencia1 "30bc2c8b-8f34-4ecc-abd6-213827478e2d"
#define CHARACTERISTIC_UUID_TX2 "405e4e26-a4de-4ff8-b2a2-c8a9465f2a6c"

#define Essencia2 "d5a74415-fc98-420d-a336-8238b4e54f5e"
#define CHARACTERISTIC_UUID_TX3 "9960735b-885f-455b-b3fd-e37f526eadd1"

#define Temperatura "1ec6580b-37ec-4205-b8ee-702c3e159347"
#define CHARACTERISTIC_UUID_TX4 "e86e8e83-2db0-4f6f-b2c3-7dfe6f3d4e22"

#define feedback_comandos "94b06173-3204-49d3-a0e9-f8fa8c762d42"
#define CHARACTERISTIC_UUID_TX5 "2825de61-5255-45ff-9bc7-0917bf54c2de"
#define CHARACTERISTIC_UUID_RX5 "a0b632f3-10e4-4917-9292-a335b1ff9409"


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
                if (rxValue.find("receita1") != -1) { 
                comando = 1;
               }
               else if (rxValue.find("receita2") != -1) {
                comando = 2;
               }
                else if (rxValue.find("receita3") != -1) { 
                comando = 3;
               }
               else if (rxValue.find("limpeza") != -1) {
                comando = 4;
               }
          }
     }
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
  tempo =((pulseIn(B,HIGH)));
}

void setup() 
{

Serial.begin(115200);
 ultrassonico(trigger,echo);
  BLEDevice::init("Maquina de Sabao");//Da nome ao servidor bluetooth

  BLEServer *server = BLEDevice::createServer();//Cria o servidor Bluetooth
  server->setCallbacks(new ServerCallbacks());//seta funcao de callback para o servidor

  BLEService *service0 = server->createService(Agua);//servico da agua
  BLEService *service1 = server->createService(Oleo);//servico do oleo
  BLEService *service2 = server->createService(Essencia1);//servico da essencia 1
  BLEService *service3 = server->createService(Essencia2);//servico da essencia 2
  BLEService *service4 = server->createService(Temperatura);//servico da temperatura
  BLEService *service5 = server->createService(feedback_comandos);//servico de comandos

  //cria as caracteristicas de notificacao.
  characteristicTX0 = service0->createCharacteristic(CHARACTERISTIC_UUID_TX0, BLECharacteristic::PROPERTY_NOTIFY);//cria as caracteristicas de notificacao.
  characteristicTX1 = service1->createCharacteristic(CHARACTERISTIC_UUID_TX1, BLECharacteristic::PROPERTY_NOTIFY);
  characteristicTX2 = service2->createCharacteristic(CHARACTERISTIC_UUID_TX2, BLECharacteristic::PROPERTY_NOTIFY);
  characteristicTX3 = service3->createCharacteristic(CHARACTERISTIC_UUID_TX3, BLECharacteristic::PROPERTY_NOTIFY);
  characteristicTX4 = service4->createCharacteristic(CHARACTERISTIC_UUID_TX4, BLECharacteristic::PROPERTY_NOTIFY);
  characteristicTX5 = service5->createCharacteristic(CHARACTERISTIC_UUID_TX5, BLECharacteristic::PROPERTY_NOTIFY);

  //adiciona descritor a caracteristica.
  characteristicTX0 ->addDescriptor(new BLE2902());
  characteristicTX1 ->addDescriptor(new BLE2902());
  characteristicTX2 ->addDescriptor(new BLE2902());
  characteristicTX3 ->addDescriptor(new BLE2902());
  characteristicTX4 ->addDescriptor(new BLE2902());
  characteristicTX5 ->addDescriptor(new BLE2902());

  BLECharacteristic *characteristic0 = service5->createCharacteristic(CHARACTERISTIC_UUID_RX5,BLECharacteristic::PROPERTY_WRITE);

    characteristic0->setCallbacks(new CharacteristicCallbacks());//seta a funcao de callback no servico 5.
    
    // Inicia os servicos
    service0->start();
    service1->start();
    service2->start();
    service3->start();
    service4->start();
    service5->start();
    
    // Inicia o advertisement (descoberta do ESP32)
    server->getAdvertising()->start();
void loop() 
{
      if (deviceConnected) {
      sonico(trigger,echo);
    dist= (tempo/29/2)-1;
    media = (dist *(1-0.8))+(media*0.8);
    altura = 8.5-media;
    
    if (altura<0)
    {
      altura = 0;
    }
    raio = 0.14705882*altura +2.25;
    volume = (3.141592*(altura)/3)*(raio*raio + raio*2.25 +2.25*2.25);
    char txString[8],txString2[8];
      
  dtostrf(volume,2,2,txString);
  //dtostrf(pagode,2,2,txString2);
  characteristicTX0->setValue(txString); //seta o valor que a caracteristica notificará (enviar) 
  characteristicTX0->notify(); 
 // characteristicTX1->setValue(txString2);
 // characteristicTX1->notify();
  Serial.print("altura: ");
  Serial.println(altura);
  Serial.print("distancia: ");
  Serial.println(media);
  Serial.print("raio: ");
  Serial.println(raio);

  delay(100);
  tempo = 0;
   }
  }
