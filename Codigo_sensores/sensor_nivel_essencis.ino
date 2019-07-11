#define trigger GPIO_NUM_26
#define echo GPIO_NUM_25

const int numLeituras = 20;
float tempo,h=0,dist,media = 0,leituras[numLeituras],volume=0;

const float Ainf=36,Asup=70; //ate 0.5L 72.25



int leituraActual = 0;
float total = 0; 


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
  leituras[leituraAtual] = dist;
  total = total + leituras[leituraAtual];
  leituraAtual = leituraAtual + 1;
  if (leituraAtual >= numLeituras)
  {
    leituraAtual = 0;
  }
  media = total / numLeituras; 
  
    h=7.85-media;

volume=(h)*(Ainf+Asup+sqrt(Ainf*Asup))/3000;

  
  
  
  Serial.print(media);
  Serial.println("cm");
  Serial.print(volume,3);
  Serial.println("L");
  
  delay(400);
}
