//int tempo;//Variavel que armazena o tempo.
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include "HardwareSerial.h"
#include "HX711.h"

BLECharacteristic *characteristicTX0, *characteristicTX1, *characteristicTX2, *characteristicTX3, *characteristicTX4, *characteristicTX5, *characteristicTX6;
HardwareSerial MySerial(1);
const int numLeituras = 20;
bool deviceConnected = false;
const float Rinf=5.6,Rsup=6.8; //ate 0.5L
const float Rinf2=7.25,Rsup2=7.8;
float tempo, volume1,volume2,volume3,volume4, dist1, dist2, media1, media2,media3,media4,h=0,dist,media = 0,leituras[numLeituras], altura, raio, Voltage=0,tempC=0.0, media7 = 0.0;
int comando, temp_digital = 0;
const float Ainf=36,Asup=70; //ate 0.5L 72.25
int leituraAtual = 0;
float total = 0;

//variaveis das quantidades de ingredientes
float vol_oleo=0,vol_etanol=0,massa_soda=0,agua_soda=0,vol_agua=0,agua_quente=0,vol_essencia1=0,vol_essencia2=0;


#define echo1 GPIO_NUM_36 //primeiro echo
#define echo2 GPIO_NUM_39 //segundo echo
#define echo3 GPIO_NUM_34 //terceiro echo
#define echo4 GPIO_NUM_35 //quarto echo
#define lmdata GPIO_NUM_32 //sensor de temperatura
#define phdata GPIO_NUM_33 //sensor de ph
#define fluxo1 GPIO_NUM_25 //sensor de fluxo 1
#define motor GPIO_NUM_26 //sensor de fluxo 2

//defines da balança
#define DOUT GPIO_NUM_27  //balanca
#define CLK GPIO_NUM_14 //balanca
#define bomba1 GPIO_NUM_23
#define valvula1 GPIO_NUM_0


#define fluxo3 GPIO_NUM_12  //sensor de fluxo 3
#define trigger GPIO_NUM_13 //trigger
#define bomba2 GPIO_NUM_22
#define bomba3 GPIO_NUM_21
#define bomba4 GPIO_NUM_19
#define bomba5 GPIO_NUM_18
#define bomba6 GPIO_NUM_5
#define bomba7 GPIO_NUM_4
#define valvula2 GPIO_NUM_2
//#define motor GPIO_NUM_15
#define tx GPIO_NUM_17
#define rx GPIO_NUM_16


#define Alcool  "2aaa9a64-6c81-4718-95eb-92fcdc1f95c8"
#define CHARACTERISTIC_UUID_TX0 "a140d3d3-4ece-4c37-9a12-37de04b65e4b"

#define Oleo "c4352ec5-7869-462b-b064-9065a9a55800"
#define CHARACTERISTIC_UUID_TX1 "17df5d0b-7068-4f0e-93d3-49033485eef5"

#define Essencias "30bc2c8b-8f34-4ecc-abd6-213827478e2d"
#define CHARACTERISTIC_UUID_TX2 "405e4e26-a4de-4ff8-b2a2-c8a9465f2a6c"
#define CHARACTERISTIC_UUID_TX3 "9960735b-885f-455b-b3fd-e37f526eadd1"

#define Temperatura "1ec6580b-37ec-4205-b8ee-702c3e159347"
#define CHARACTERISTIC_UUID_TX4 "e86e8e83-2db0-4f6f-b2c3-7dfe6f3d4e22"

#define Soda "c267d9d8-7022-48f1-a079-4474e99a81d5"
#define CHARACTERISTIC_UUID_TX6 "05ef93f2-7477-43cf-ab64-20ceb0609e82"

#define feedback_comandos "94b06173-3204-49d3-a0e9-f8fa8c762d42"
#define CHARACTERISTIC_UUID_TX5 "2825de61-5255-45ff-9bc7-0917bf54c2de"
#define CHARACTERISTIC_UUID_RX5 "a0b632f3-10e4-4917-9292-a335b1ff9409"



void configura_bomba_solenoide()
{
    gpio_set_direction(bomba1  , GPIO_MODE_OUTPUT);//configura bomba1 p/ BOMBA Da balança
    digitalWrite(bomba1, LOW);
    gpio_set_direction(bomba2  , GPIO_MODE_OUTPUT);//configura bomba2 p/ BOMBA Do oleo 
    digitalWrite(bomba2, LOW);
    gpio_set_direction(bomba3 , GPIO_MODE_OUTPUT);//configura bomba3 p/ BOMBA Do alcool
    digitalWrite(bomba3, LOW);
    gpio_set_direction(bomba4 , GPIO_MODE_OUTPUT);//configura bomba4 p/ BOMBA Da essencia1
    digitalWrite(bomba4, LOW);
    gpio_set_direction(bomba5 , GPIO_MODE_OUTPUT);//configura bomba4 p/ BOMBA Da essencia2
    digitalWrite(bomba5, LOW);
    gpio_set_direction(bomba6 , GPIO_MODE_OUTPUT);//configura bomba4 p/ BOMBA Da agua quente
    digitalWrite(bomba6, LOW);

    gpio_set_direction(valvula1 , GPIO_MODE_OUTPUT);//configura valvula p/  VALVULA Da balança
    digitalWrite(valvula1, LOW);
    gpio_set_direction(valvula2 , GPIO_MODE_OUTPUT);//configura valvula p/  VALVULA Da agua quente
    digitalWrite(valvula2, LOW);
    

    gpio_set_direction(motor , GPIO_MODE_OUTPUT);//configura saida para ligar motor
    digitalWrite(motor, LOW);

      
}

void rotina_oleo()
{
    characteristicTX5->setValue("2");
    characteristicTX5->notify();

    digitalWrite(bomba2, HIGH);
    delay(vol_oleo);             //tempo em milisegundos necessario para escoar o volume definido
    digitalWrite(bomba2, LOW); 
  
}

void rotina_alcool()
{   
    characteristicTX5->setValue("3");
    characteristicTX5->notify();
    digitalWrite(bomba3, HIGH);
    delay(vol_etanol);             //tempo em milisegundos necessario para escoar o volume definido
    digitalWrite(bomba3, LOW); 
}

void rotina_agua_quente()
{
    characteristicTX5->setValue("4");
    characteristicTX5->notify();
    digitalWrite(bomba6, HIGH);
    delay(4000);             //tempo em milisegundos necessario para escoar o volume definido
    digitalWrite(bomba6, LOW); 
}



void rotina_essencia_1()
{

  
    characteristicTX5->setValue("10");
    characteristicTX5->notify();
    digitalWrite(bomba4, HIGH);
    delay(vol_essencia1);             //tempo em milisegundos necessario para escoar o volume definido
    digitalWrite(bomba4, LOW); 
}

void rotina_essencia_2()
{
    
    characteristicTX5->setValue("10");
    characteristicTX5->notify();
    digitalWrite(bomba5, HIGH);
    delay(vol_essencia2);             //tempo em milisegundos necessario para escoar o volume definido
    digitalWrite(bomba5, LOW); 
}



void rotina_motor()
{
    characteristicTX5->setValue("5");
    characteristicTX5->notify();
    digitalWrite(motor, HIGH);
    delay(10000);             //tempo em milisegundos necessario para escoar o volume definido
    digitalWrite(motor, LOW); 
}

void finale()
{

    characteristicTX5->setValue("FIM PH");
    characteristicTX5->notify();
    delay(5000);
}


HX711 balanca;
float calibration_factor = 984130;
void inicia_balanca()
{
    balanca.begin(DOUT, CLK);                          // inicializa a balança
    balanca.set_scale(calibration_factor);   // ajusta fator de calibração
    balanca.tare();   // zera a Balança  
    
}

void rotina_soda_agua()
{
  characteristicTX5->setValue("1");
  characteristicTX5->notify();
  delay(100);
  int controle=0;
  char txString[8];
  float peso_soda=0, y=0, massa_agua=0;
  y=massa_soda+agua_soda; //agua_sooda em mL
  massa_agua=vol_agua;
  if (vol_agua>1000)
  {
      massa_agua=(vol_agua*1000)/2;
  }
  
  
  while(1)
  {

    
    peso_soda=balanca.get_units();

    if (controle==0){
      
    dtostrf(peso_soda, 2, 2, txString);
    characteristicTX6->setValue("txString");
    characteristicTX6->notify();
     }
    
    Serial.println(balanca.get_units(), 3); 
    
    if ((peso_soda>=massa_soda) && (controle==0))
      {  
        digitalWrite(valvula1,HIGH);
        Serial.print("meupau");
        controle=10; 
      }
    else if((peso_soda>=y)&&((controle==10)))
     {
        digitalWrite(valvula1,LOW);
        digitalWrite(bomba1,HIGH);
        Serial.print("pirutorto");
        controle=20;
       
     }
   
   
   else if((peso_soda<=0.005) &&(controle==20))
     {
        digitalWrite(bomba1,LOW);
        digitalWrite(valvula1,HIGH);
        controle=30;
        Serial.print("papaco");
     } 
    if((peso_soda>=massa_agua) && (controle==30))
     {
        digitalWrite(valvula1,LOW);            
        digitalWrite(bomba1,HIGH);
        controle=40;
        
     }
    if((peso_soda<=0.005) && (controle==40))
     {  
        digitalWrite(valvula1,LOW);
        digitalWrite(bomba1,LOW);
        if(vol_agua<=1)
        {
        controle=70;
        }
        else if(vol_agua>=1)
        {
           digitalWrite(valvula1,HIGH);
           digitalWrite(bomba1,LOW); 
           controle=50;
        }
     }
    if( (peso_soda>=massa_agua) && (controle==50))
     {  
           digitalWrite(valvula1,LOW);
           digitalWrite(bomba1,HIGH); 
           controle=60;     
     }
     if((peso_soda<=0.005) && (controle==60))
     {
           digitalWrite(valvula1,LOW);
           digitalWrite(bomba1,LOW); 
           controle=70;
     }
     if (controle==70)
     {
           controle=0;
           break;
     }
   delay(500);  
   }   
  
}




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
        if (rxValue.find("receita10") != -1)
        {
          comando = 1;
        }
        else if (rxValue.find("receita20") != -1)
        {
          comando = 2;
        }
        else if (rxValue.find("receita30") != -1)
        {
          comando = 3;
        }
        else if (rxValue.find("receita11") != -1)
        {
          comando = 4;
        }
        else if (rxValue.find("receita21") != -1)
        {
          comando = 5;
        }
        else if (rxValue.find("receita31") != -1)
        {
          comando = 6;
        }
        else if (rxValue.find("receita12") != -1)
        {
          comando = 7;
        }
        else if (rxValue.find("receita22") != -1) {
          comando = 8;
        }
        else if (rxValue.find("receita32") != -1) {
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

void ultrassonico(gpio_num_t A, gpio_num_t B, gpio_num_t C, gpio_num_t D, gpio_num_t E)
{
  gpio_set_direction(A, GPIO_MODE_OUTPUT);
  gpio_set_direction(B, GPIO_MODE_INPUT);
  gpio_set_direction(C, GPIO_MODE_INPUT);
  gpio_set_direction(D, GPIO_MODE_INPUT);
  gpio_set_direction(E, GPIO_MODE_INPUT);
}
void sonico(gpio_num_t A, gpio_num_t B)
{
  gpio_set_level(A, 0);
  delayMicroseconds(2);
  gpio_set_level(A, 1);
  delayMicroseconds(10);
  gpio_set_level(A, 0);
  tempo = ((pulseIn(B, HIGH)));
}
void nivel_alcool(gpio_num_t A, gpio_num_t B)
{
   total = total - leituras[leituraAtual];
  sonico(A,B);
  dist= tempo*0.034/2;
  leituras[leituraAtual] = dist;
  total = total + leituras[leituraAtual];
  leituraAtual = leituraAtual + 1;
  if (leituraAtual >= numLeituras)
  {
    leituraAtual = 0;
  }
  media1 = total / numLeituras; 
  
    h=11.1-media;

  if (media>6.95)
     {
      
      volume1= (3.1415926*h*(Rinf*Rinf + Rinf*Rsup + Rsup*Rsup )/3000);
     }
  else
     {
        volume1= 0.5+(3.1415926*(6.95-media)*(Rinf2*Rinf2 + Rinf2*Rsup2 + Rsup2*Rsup2 )/3000);
      //  volume= 0.5 +(3.1415926*(6.95-media)*7.5*7.5)/1000;
}
}
void nivel_oleo(gpio_num_t A, gpio_num_t B)
{
  total = total - leituras[leituraAtual];
  sonico(A,B);
  dist= tempo*0.034/2;
  leituras[leituraAtual] = dist;
  total = total + leituras[leituraAtual];
  leituraAtual = leituraAtual + 1;
  if (leituraAtual >= numLeituras)
  {
    leituraAtual = 0;
  }
  media = total / numLeituras; 
  
    h=7.85-media;

volume1=(h)*(Ainf+Asup+sqrt(Ainf*Asup))/3000;
}
void nivel_essencias(gpio_num_t A, gpio_num_t B)
{
  total = total - leituras[leituraAtual];
  sonico(A,B);
  dist= tempo*0.034/2;
  leituras[leituraAtual] = dist;
  total = total + leituras[leituraAtual];
  leituraAtual = leituraAtual + 1;
  if (leituraAtual >= numLeituras)
  {
    leituraAtual = 0;
  }
  media = total / numLeituras; 
  
    h=7.85-media;

volume1=(h)*(Ainf+Asup+sqrt(Ainf*Asup))/3000;
}
void setup()
{

  MySerial.begin(115200, SERIAL_8N1, rx, tx);
  MySerial.write(1);
  ultrassonico(trigger, echo1, echo2, echo3, echo4);
  delay(10);
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

  BLECharacteristic *characteristic0 = service6->createCharacteristic(CHARACTERISTIC_UUID_RX5, BLECharacteristic::PROPERTY_WRITE); 
                    BLECharacteristic *characteristic2 = service2->createCharacteristic(CHARACTERISTIC_UUID_TX3, BLECharacteristic::PROPERTY_READ);

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

//inicia_sensores_solenoides_bombas

  inicia_balanca(); //iniciando a balança
  configura_bomba_solenoide();



  xTaskCreatePinnedToCore(loop2, "loop2", 8192, NULL, 2, NULL, 1);//Cria a tarefa "loop2()" com prioridade 1, atribuída ao core 0
  delay(1); //delayzinho só pra não acionar o watchdog timer
}

void loop()//O loop() sempre será atribuído ao core 1 automaticamente pelo sistema, com prioridade 1
{
  // coloque aqui os códigos para mandar os níveis dos reservatórios e da temperatura do tanque para o aplicativo.
  //Lembre-se de transformar tudo para string antes de mandar
  total = total - leituras[leituraAtual];
  sonico(trigger,echo1);
  dist= tempo*0.034/2;
  leituras[leituraAtual] = dist;
  total = total + leituras[leituraAtual];
  leituraAtual = leituraAtual + 1;
  if (leituraAtual >= numLeituras)
  {
    leituraAtual = 0;
  }
  media = total / numLeituras; 
  
    h=7.85-media;

volume1=(h)*(Ainf+Asup+sqrt(Ainf*Asup))/3000;

   for(int i = 0; i<10; i++)
  {
    temp_digital = analogRead(lmdata);   //leitura da gpio 35 da esp32
    media7 += temp_digital/10;
  }
    Voltage = (media7 / 4095.0) * 3300;
    tempC = Voltage * 0.1; 


 // media1 = (dist1 * (1 - 0.8)) + (media1 * 0.8);
  //media2 = (dist2 * (1 - 0.8)) + (media2 * 0.8);
  //  altura = 8.5-media1;
  //  if (altura<0)
  //  {
  //    altura = 0;
  //}
  // raio = 0.14705882*altura +2.25;
  // volume = (3.141592*(altura)/3)*(raio*raio + raio*2.25 +2.25*2.25);
  char txString[8], txString2[8];

  dtostrf(temp_digital, 2, 2, txString);
  dtostrf(tempC, 2, 2, txString2);
  characteristicTX0->setValue(txString); //seta o valor que a caracteristica notificará (enviar)
  characteristicTX0->notify();
  characteristicTX4->setValue(txString2); //seta o valor que a caracteristica notificará (enviar)
  characteristicTX4->notify();
  media7=0;
  //Voltage = 0.0;
  //tempC = 0.0;
  //temp_digital=0;
  delay(200);//Mantem o processador 1 em estado ocioso por 0,1seg
}

void loop2(void*z)//Atribuímos o loop2 ao core 0, com prioridade 1
{
  
  //aqui é onde se iniciam os processos da máquina. Em cada case terá uma dosagem diferente a depender da receita.
  //Por segurança eu sempre zero a variável "comando" para a máquina não ter que fazer a receita de novo. recomendo deixar um delay
  //depois de zerar esta variável. Sempre informar também por meio da característica TX5 as etapas da lavagem. No rela tem essas etapas.
  while (1)
  {
  
    while(comando==0)
    {
      
    characteristicTX5->setValue("pode comecar");
    characteristicTX5->notify(); 

      vol_oleo=0;       // fazer tempo na bomba
      vol_etanol=0;   // fazer tempo na bomba
      massa_soda=0;   // massa da soda em kg
      agua_soda=0;     // Em litros(SENSOR DE BALANÇA)    
      vol_agua=0;       // Em litros(SENSOR DE BALANÇA)
      agua_quente=0;    // Em litros(SENSOR DE FLUXA)
      vol_essencia1=0;    // Em mL (TEMPO NECESSARIO-CALCULAR)
      vol_essencia2=0;    // Em mL(TEMPO NECESSARIO-CALCULAR)
   
    }
   delay(100);
   switch (comando)
    {
      case 1://receita 1 sem essência
          vol_oleo=2000;       // fazer tempo na bomba
          vol_etanol=4000;   // fazer tempo na bomba
          massa_soda=0.150;   // massa da soda em kg
          agua_soda=0.400;     // Em litros(SENSOR DE BALANÇA)    
          vol_agua=0.400;       // Em litros(SENSOR DE BALANÇA)
          agua_quente=1000;    // Em litros(SENSOR DE FLUXA)
          vol_essencia1=0;    // Em mL (TEMPO NECESSARIO-CALCULAR)
          vol_essencia2=0;     // Em mL(TEMPO NECESSARIO-CALCULAR)
          comando = 0;      
          break;
      case 2://receita 2 sem essência
         characteristicTX5->setValue("1");
        characteristicTX5->notify();
          vol_oleo=0.5;       // fazer tempo na bomba
          vol_etanol=0.250;   // fazer tempo na bomba
          massa_soda=0.200;   // massa da soda em kg
          agua_soda=0.450 ;    // Em litros(SENSOR DE BALANÇA)    
          vol_agua=0.400;       // Em litros(SENSOR DE BALANÇA)
          agua_quente=0.5;    // Em litros(SENSOR DE FLUXA)
          vol_essencia1=0;    // Em mL (TEMPO NECESSARIO-CALCULAR)
          vol_essencia2=0 ;    // Em mL(TEMPO NECESSARIO-CALCULAR)
          comando = 0;      
       
        break;
  
      case 3://receita 3 sem essência
          vol_oleo=1;       // fazer tempo na bomba
          vol_etanol=0.500;   // fazer tempo na bomba
          massa_soda=250;   // massa da soda em kg
          agua_soda=0.500;     // Em litros(SENSOR DE BALANÇA)    
          vol_agua=4.5;       // Em litros(SENSOR DE BALANÇA)
          agua_quente=1.5;    // Em litros(SENSOR DE FLUXA)
          vol_essencia1=0;    // Em mL (TEMPO NECESSARIO-CALCULAR)
          vol_essencia2=0;   // Em mL(TEMPO NECESSARIO-CALCULAR)
          comando = 0;      
        break;
  
      case 4://receita 1 com a essencia 1
          vol_oleo=4000;       // fazer tempo na bomba
          vol_etanol=3000;   // fazer tempo na bomba
          massa_soda=0.150;   // massa da soda em kg
          agua_soda=0.400 ;    // Em litros(SENSOR DE BALANÇA)    
          vol_agua=0.400;       // Em litros(SENSOR DE BALANÇA)
          agua_quente=4000;    // Em litros(SENSOR DE FLUXA)
          vol_essencia1=1000;    // Em mL (TEMPO NECESSARIO-CALCULAR)
          vol_essencia2=0 ;    // Em mL(TEMPO NECESSARIO-CALCULAR)
          comando = 0;      
          break;
      case 5://receita 2 com a essencia 1
          vol_oleo=0.25;       // fazer tempo na bomba
          vol_etanol=0.250;   // fazer tempo na bomba
          massa_soda=0.150;   // massa da soda em kg
          agua_soda=0.400;     // Em litros(SENSOR DE BALANÇA)    
          vol_agua=0.400;       // Em litros(SENSOR DE BALANÇA)
          agua_quente=0.5;    // Em litros(SENSOR DE FLUXA)
          vol_essencia1=20;    // Em mL (TEMPO NECESSARIO-CALCULAR)
          vol_essencia2=0 ;    // Em mL(TEMPO NECESSARIO-CALCULAR)
          comando = 0;      
          break;
      case 6://receita 3 com a essencia 1
          characteristicTX5->setValue("1");
          characteristicTX5->notify();
          vol_oleo=1;       // fazer tempo na bomba
          vol_etanol=0.500;   // fazer tempo na bomba
          massa_soda=250;   // massa da soda em kg
          agua_soda=0.500;     // Em litros(SENSOR DE BALANÇA)    
          vol_agua=4.5;       // Em litros(SENSOR DE BALANÇA)
          agua_quente=1.5;    // Em litros(SENSOR DE FLUXA)
          vol_essencia1=0;    // Em mL (TEMPO NECESSARIO-CALCULAR)
          vol_essencia2=0;     // Em mL(TEMPO NECESSARIO-CALCULAR)
          comando = 0;      
        break;

      case 7://receita 1 com a essencia 2
          vol_oleo=0.25;       // fazer tempo na bomba
          vol_etanol=0.250;   // fazer tempo na bomba
          massa_soda=0.150;   // massa da soda em kg
          agua_soda=0.400 ;    // Em litros(SENSOR DE BALANÇA)    
          vol_agua=0.400;       // Em litros(SENSOR DE BALANÇA)
          agua_quente=0.5;    // Em litros(SENSOR DE FLUXA)
          vol_essencia1=20;    // Em mL (TEMPO NECESSARIO-CALCULAR)
          vol_essencia2=0 ;   // Em mL(TEMPO NECESSARIO-CALCULAR)
          comando = 0;      
          break;
      case 8://receita 2 com a essencia 2
          vol_oleo=0.25;       // fazer tempo na bomba
          vol_etanol=0.250;   // fazer tempo na bomba
          massa_soda=0.150;   // massa da soda em kg
          agua_soda=0.400;     // Em litros(SENSOR DE BALANÇA)    
          vol_agua=0.400;       // Em litros(SENSOR DE BALANÇA)
          agua_quente=0.5;    // Em litros(SENSOR DE FLUXA)
          vol_essencia1=20;    // Em mL (TEMPO NECESSARIO-CALCULAR)
          vol_essencia2=0;     // Em mL(TEMPO NECESSARIO-CALCULAR)
          comando = 0;      
          break;
      case 9://receita 3 com a essencia 2
          characteristicTX5->setValue("1");
          characteristicTX5->notify();
          vol_oleo=1;       // fazer tempo na bomba
          vol_etanol=0.500;   // fazer tempo na bomba
          massa_soda=250;   // massa da soda em kg
          agua_soda=0.500;     // Em litros(SENSOR DE BALANÇA)    
          vol_agua=4.5;       // Em litros(SENSOR DE BALANÇA)
          agua_quente=1.5;    // Em litros(SENSOR DE FLUXA)
          vol_essencia1=0;    // Em mL (TEMPO NECESSARIO-CALCULAR)
          vol_essencia2=0;    // Em mL(TEMPO NECESSARIO-CALCULAR)
          comando = 0;      
        break;       
        default:
          comando = 0;
        break;
    
    }
    //aqui inicia a maquina após selecionar os ingredientes;
    // rotina_soda_agua();  //rotina para dosar a quantidade de agua, soda.
    rotina_oleo(); 
    rotina_alcool();
    rotina_essencia_1();
      //if(vol_essencia1>0)
      //{
           rotina_essencia_1();
      //}
      //else if(vol_essencia2>0)
      //{
     //     rotina_essencia_2();  
      //}
      rotina_agua_quente();
      rotina_motor(); 
     
      finale();
      comando=0;
      delay(100);

}


}
