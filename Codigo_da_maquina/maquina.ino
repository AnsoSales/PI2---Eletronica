//int tempo;//Variavel que armazena o tempo.
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

BLECharacteristic *characteristicTX0, *characteristicTX1, *characteristicTX2, *characteristicTX3,*characteristicTX4, *characteristicTX5, *characteristicTX6;

bool deviceConnected = false;
float tempo,volume,dist1,dist2,media1,media2,altura,raio = 0.0;
int comando = 0;

#define trigger GPIO_NUM_13
#define echo1 GPIO_NUM_36
#define echo2 GPIO_NUM_39
#define echo3 GPIO_NUM_34
#define echo4 GPIO_NUM_35


#define Alcool  "2aaa9a64-6c81-4718-95eb-92fcdc1f95c8"
#define CHARACTERISTIC_UUID_TX0 "a140d3d3-4ece-4c37-9a12-37de04b65e4b"

#define Oleo "c4352ec5-7869-462b-b064-9065a9a55800"
#define CHARACTERISTIC_UUID_TX1 "17df5d0b-7068-4f0e-93d3-49033485eef5"

#define Essencias "30bc2c8b-8f34-4ecc-abd6-213827478e2d"
#define CHARACTERISTIC_UUID_TX2 "405e4e26-a4de-4ff8-b2a2-c8a9465f2a6c"
#define CHARACTERISTIC_UUID_TX3 "9960735b-885f-455b-b3fd-e37f526eadd1"

#define Temperatura "1ec6580b-37ec-4205-b8ee-702c3e159347"
#define CHARACTERISTIC_UUID_TX4 "e86e8e83-2db0-4f6f-b2c3-7dfe6f3d4e22"
#define CHARACTERISTIC_UUID_TX7 "a60744fa-dee3-48ec-ad65-8c999eb4a3f7"

#define Soda "c267d9d8-7022-48f1-a079-4474e99a81d5"
#define CHARACTERISTIC_UUID_TX6 "05ef93f2-7477-43cf-ab64-20ceb0609e82"

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
             //Serial.print(rxValue[i]);
               }
              // Serial.println();
                if (rxValue.find("receita1-") != -1) 
               { 
                comando = 1;
               }
               else if (rxValue.find("receita2-") != -1) 
               {
                comando = 2;
               }
                else if (rxValue.find("receita3-") != -1) 
                { 
                comando = 3;
               }
               else if (rxValue.find("receita1-0") != -1) 
               {
                comando = 4;
               }
                else if (rxValue.find("receita2-0") != -1) 
                { 
                comando = 5;
               }
               else if (rxValue.find("receita3-0") != -1) 
               {
                comando = 6;
               }
                else if (rxValue.find("receita1-1") != -1) 
               { 
                comando = 7;
               }
               else if (rxValue.find("receita2-1") != -1) {
                comando = 8;
               }
                else if (rxValue.find("receita3-1") != -1) { 
                comando = 9;
               }
               else if (rxValue.find("limpeza") != -1) {
                comando = 10;
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
  Serial.begin(115200);//Inicia a comunicaçao serial
  pinMode(13, OUTPUT);//Define o led Onboard como saída
  ultrassonico(trigger,echo1);
  delay(10);
  ultrassonico(trigger,echo2);
  BLEDevice::init("Maquina de Sabao");//Da nome ao servidor bluetooth

  BLEServer *server = BLEDevice::createServer();//Cria o servidor Bluetooth
  server->setCallbacks(new ServerCallbacks());//seta funcao de callback para o servidor

  BLEService *service0 = server->createService(Alcool);//servico da agua
  BLEService *service1 = server->createService(Oleo);//servico do oleo
  BLEService *service2 = server->createService(Essencias);//servico da essencia 1
  BLEService *service4 = server->createService(Temperatura);//servico da temperatura
  BLEService *service5 = server->createService(Soda);//servico de comandos
  BLEService *service6 = server->createService(feedback_comandos);//servico da soda
  
  //cria as caracteristicas de notificacao.
  characteristicTX0 = service0->createCharacteristic(CHARACTERISTIC_UUID_TX0, BLECharacteristic::PROPERTY_READ);//cria as caracteristicas de notificacao.
  characteristicTX1 = service1->createCharacteristic(CHARACTERISTIC_UUID_TX1, BLECharacteristic::PROPERTY_READ);
  characteristicTX2 = service2->createCharacteristic(CHARACTERISTIC_UUID_TX2, BLECharacteristic::PROPERTY_READ);
  characteristicTX4 = service4->createCharacteristic(CHARACTERISTIC_UUID_TX4, BLECharacteristic::PROPERTY_READ);
  characteristicTX6 = service5->createCharacteristic(CHARACTERISTIC_UUID_TX6, BLECharacteristic::PROPERTY_READ);
  characteristicTX5 = service6->createCharacteristic(CHARACTERISTIC_UUID_TX5, BLECharacteristic::PROPERTY_READ);

  //adiciona descritor a caracteristica.
  characteristicTX0 ->addDescriptor(new BLE2902());
  characteristicTX1 ->addDescriptor(new BLE2902());
  characteristicTX2 ->addDescriptor(new BLE2902());
  characteristicTX4 ->addDescriptor(new BLE2902());
  characteristicTX5 ->addDescriptor(new BLE2902());
  characteristicTX6 ->addDescriptor(new BLE2902());
  
  BLECharacteristic *characteristic0 = service6->createCharacteristic(CHARACTERISTIC_UUID_RX5,BLECharacteristic::PROPERTY_WRITE);
  BLECharacteristic *characteristic1 = service4->createCharacteristic(CHARACTERISTIC_UUID_TX7,BLECharacteristic::PROPERTY_READ);
  BLECharacteristic *characteristic2 = service2->createCharacteristic(CHARACTERISTIC_UUID_TX3,BLECharacteristic::PROPERTY_READ);

    characteristic0->setCallbacks(new CharacteristicCallbacks());//seta a funcao de callback no servico 5.
    
    // Inicia os servicos
    service0->start();
    service1->start();
    service2->start();
    service4->start();
    service5->start();
    service6->start();
    
    // Inicia o advertisement (descoberta do ESP32)
    server->getAdvertising()->start();
 
 
  Serial.printf("\nsetup() em core: %d", xPortGetCoreID());//Mostra no monitor em qual core o setup() foi chamado
  xTaskCreatePinnedToCore(loop2, "loop2", 8192, NULL, 1, NULL, 0);//Cria a tarefa "loop2()" com prioridade 1, atribuída ao core 0
  delay(1);
}
 
void loop()//O loop() sempre será atribuído ao core 1 automaticamente pelo sistema, com prioridade 1
{
    // coloque aqui os códigos para mandar os níveis dos reservatórios e da temperatura do tanque para o aplicativo. 
     //Lembre-se de transformar tudo para string antes de mandar
  Serial.printf("\n Tempo corrido: %d");
  sonico(trigger,echo1);
  dist1= (tempo/29/2)-1;
  delay(70);
  sonico(trigger,echo2);
  dist2= (tempo/29/2)-1;
  delay(70);
    media1 = (dist1 *(1-0.8))+(media1*0.8);
    media2 = (dist2 *(1-0.8))+(media2*0.8);
    altura = 8.5-media1;
    if (altura<0)
    {
      altura = 0;
   }
    raio = 0.14705882*altura +2.25;
    volume = (3.141592*(altura)/3)*(raio*raio + raio*2.25 +2.25*2.25);
    char txString[8],txString2[8];
      
  dtostrf(media1,2,2,txString);
  dtostrf(media2,2,2,txString2);
  characteristicTX0->setValue(txString); //seta o valor que a caracteristica notificará (enviar) 
  characteristicTX0->notify(); 
  characteristicTX1->setValue(txString2); //seta o valor que a caracteristica notificará (enviar) 
  characteristicTX1->notify();
  delay(100);//Mantem o processador 1 em estado ocioso por 0,1seg
}
 
void loop2(void*z)//Atribuímos o loop2 ao core 0, com prioridade 1
{
    characteristicTX5->setValue("pode comecar");
    characteristicTX5->notify();
       //aqui é onde se inicia 
    switch (comando)
    {
      case 1:
      digitalWrite(13, HIGH);
      characteristicTX5->setValue("limpeza");
      characteristicTX5->notify();
      delay(2000);
      digitalWrite(13, LOW);
      characteristicTX5->setValue("limpeza Concluida");
      characteristicTX5->notify();
      comando = 0;
      delay(2000);
      break;
      case 2:
      digitalWrite(13, HIGH);
      characteristicTX5->setValue("4");
      characteristicTX5->notify();
      delay(2000);
      characteristicTX5->setValue("3");
      characteristicTX5->notify();
      delay(2000);
      characteristicTX5->setValue("2");
      characteristicTX5->notify();
      delay(2000);
      characteristicTX5->setValue("1");
      characteristicTX5->notify();
      delay(2000);
      characteristicTX5->setValue("FIM");
      characteristicTX5->notify();
      delay(2000);
      digitalWrite(13, LOW);
      comando = 0;
      break;
      default:
      digitalWrite(13, HIGH);
      characteristicTX5->setValue("pode comecar");
      characteristicTX5->notify();
      delay(6000);
      digitalWrite(13, LOW);
      delay(6000);
      break;
    } 
    delay(100);
  }
}
