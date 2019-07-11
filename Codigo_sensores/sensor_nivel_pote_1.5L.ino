#define trigger GPIO_NUM_26
#define echo GPIO_NUM_25

const int numLeituras = 20;
float tempo,h=0,dist,media = 0,leituras[numLeituras],volume=0;

//const float Ainf=130,Asup=300; //ate 0.5L 72.25

const float Rinf=5.6,Rsup=6.8; //ate 0.5L

const float Rinf2=7.25,Rsup2=7.8;



int lecturaActual = 0; 
float total = 0


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
 
Serial.begin(115200); 
  ultrassonico(trigger,echo);
  for (int i = 0; i < numLeituras; i++)
  {
    leituras[i] = 0;
  }



}

void loop() {
  total = total - leituras[leituraAtual];
  sonico(trigger,echo);
  dist= tempo*0.034/2;
  lecturas[lecturaActual] = dist;
  total = total + lecturas[lecturaActual];
  lecturaActual = lecturaActual + 1;
  if (lecturaActual >= numLecturas)
  {
    leituraAtual = 0;
  }
  media = total / numLeituras; 
  
    h=11.1-media;

  if (media>6.95)
     {
      
      volume= (3.1415926*h*(Rinf*Rinf + Rinf*Rsup + Rsup*Rsup )/3000);
     }
  else
     {
        volume= 0.5+(3.1415926*(6.95-media)*(Rinf2*Rinf2 + Rinf2*Rsup2 + Rsup2*Rsup2 )/3000);
      //  volume= 0.5 +(3.1415926*(6.95-media)*7.5*7.5)/1000;
      }
  

  
  
  
  Serial.print(media);
  Serial.println("cm");
  Serial.print(volume,3);
  Serial.println("L");
  
  delay(400);
}
