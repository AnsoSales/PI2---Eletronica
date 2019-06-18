#include "HX711.h"

#define DOUT GPIO_NUM_32  //balanca
#define CLK GPIO_NUM_33 //balanca
#define bomba1 GPIO_NUM_23
#define valvula1 GPIO_NUM_0



HX711 balanca;
float calibration_factor = 984130;
float vol_oleo=0,vol_etanol=0,massa_soda=0.1,agua_soda=0.1,vol_agua=0.2,agua_quente=0,vol_essencia1=0,vol_essencia2=0;

void inicia_balanca()
{
    balanca.begin(DOUT, CLK);                          // inicializa a balança
    balanca.set_scale(calibration_factor);   // ajusta fator de calibração
    balanca.tare();   // zera a Balança  
    
}
void inicia_solenoide_bomba()
{
    gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT); //bomba1
    digitalWrite(valvula1,LOW);
    gpio_set_direction(GPIO_NUM_0, GPIO_MODE_OUTPUT); //valvula1
    digitalWrite(bomba1,LOW);
}    

void rotina_soda_agua()
{
  int controle=0;
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
void setup() {
  Serial.begin(9600);
 
 inicia_balanca();
 inicia_solenoide_bomba(); 

}

void loop() 
{
 rotina_soda_agua();
Serial.print("finalizado");
delay(2000);
}
