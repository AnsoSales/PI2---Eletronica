#include "HX711.h"

#define DOUT GPIO_NUM_27  //balanca
#define CLK GPIO_NUM_14 //balanca
#define bomba1 GPIO_NUM_23
#define valvula1 GPIO_NUM_0



HX711 balanca;
float calibration_factor = 945130;
float vol_oleo=0,vol_etanol=0,massa_soda=0,agua_soda=0,vol_agua=0,agua_quente=0,vol_essencia1=0,vol_essencia2=0;

void inicia_balanca()
{
    balanca.begin(DOUT, CLK);                          // inicializa a balança
    balanca.set_scale(calibration_factor);   // ajusta fator de calibração
    balanca.tare();   // zera a Balança  
}
void inicia_solenoide_bomba()
{
    gpio_set_direction(GPIO_NUM_23, GPIO_MODE_OUTPUT); //bomba1
    gpio_set_direction(GPIO_NUM_0, GPIO_MODE_OUTPUT); //valvula1
}    

void rotina_soda_agua()
{
  int controle=0;
  float peso_soda=0, y=0, massa_agua=0;
  y=massa_soda+agua_soda; //agua_sooda em mL
  massa_agua=vol_agua*1000;
  if (vol_agua>1000)
  {
      massa_agua=(vol_agua*1000)/2;
  }
  
  
  while(1)
  {
    peso_soda=balanca.get_units(); 
    if ((peso_soda>massa_soda) and (controle=0))
      {  
        digitalWrite(valvula1,HIGH);
        controle=1; 
      }
    else if((peso_soda>y) and (controle=1))
     {
        digitalWrite(valvula1,LOW);
        digitalWrite(bomba1,HIGH);
        controle=2;
     }
    else if((peso_soda<5) and (controle=2))
     {
        digitalWrite(bomba1,LOW);
        digitalWrite(valvula1,HIGH);
        controle=3;
     } 
    else if((peso_soda>massa_agua) and (controle=3))
     {
        digitalWrite(valvula1,LOW);            
        digitalWrite(bomba1,HIGH);
        controle=4;
        
     }
     else if((peso_soda<5) and (controle=4))
     {  
        digitalWrite(valvula1,LOW);
        digitalWrite(bomba1,LOW);
        if(vol_agua<1)
        {
        controle=7;
        }
        else if(vol_agua>1)
        {
           digitalWrite(valvula1,HIGH);
           digitalWrite(bomba1,LOW); 
           controle=5;
         }
     }
     else if( (peso_soda>massa_agua) and (controle=5))
     {  
           digitalWrite(valvula1,LOW);
           digitalWrite(bomba1,HIGH); 
           controle=6;     
     }
     else if((peso_soda<5) and (controle=6))
     {
           digitalWrite(valvula1,LOW);
           digitalWrite(bomba1,LOW); 
           controle=7;
     }
     else if (controle=7)
     {
           controle=0;
           break;
      }
   }   
  
}
void setup() {
 inicia_balanca();
 inicia_solenoide_bomba(); 

}

void loop() 
{
 
}
