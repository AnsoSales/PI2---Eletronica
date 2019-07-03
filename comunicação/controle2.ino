#include  <OneWire.h>
#include <DallasTemperature.h>

//Porta ligada ao pino IN1 do modulo
int porta_rele = 3;
//Armazena o estado do rele : 0 (LOW) ou 1 (HIGH)
int estadorele = 1;
int comando = 0;

float leitura;

//Declaramos o pino onde se vai ligar o cabo DATA do sensor de temp
#define PinTemp 2
 
//Declaramos o pino como bus para a comunicação OneWire
OneWire ourWire(PinTemp);
 
//Iniciamos 1 instancia da livraria DallasTemperature
DallasTemperature sensors(&ourWire);
 
//Sensor temp
//Esta função executa-se quando se inicia o Arduino
void setup() {
//Iniciamos a comunicação serial
Serial.begin(115200);
//Iniciamos o sensor
sensors.begin();
//Pequena pausa
delay(1000);

//Modulo rele
//Define pinos para o rele como saida
  pinMode(porta_rele, OUTPUT); 
  //Estado inicial do rele - desligado
  digitalWrite(porta_rele, HIGH);
}


//Sensor temp
//Esta função executa-se continuamente em laço
void loop() {
//Enviamos uma petição ao sensor para que nos devolva a temperatura
sensors.requestTemperatures();
//Lê-se e imprime a temperatura em graus Celsius no monitor serie
leitura = sensors.getTempCByIndex(0);
Serial.print(leitura);

//Modulo rele
if (Serial.available()>0)
{
 comando = Serial.read();
 if(comando = '1'){ 
  if (leitura<80){  
        digitalWrite(porta_rele, HIGH); //LIGA O MÓDULO RELÉ 
        estadorele = 0; //VARIÁVEL RECEBE O VALOR 0
        delay(200); //INTERVALO DE 200 MILISSEGUNDOS
        Serial.write(4);
        
 }else{
  //SENÃO, FAZ
            digitalWrite(porta_rele, LOW); //DESLIGA O MÓDULO RELÉ
            porta_rele = 0; //VARIÁVEL RECEBE O VALOR 0
            delay(200); //INTERVALO DE 200 MILISSEGUNDOS
            Serial.write(5);
            
        }
        delay(1000); 
//Pequena pausa de 1 segundo antes da próxima leitura
}
}
}
