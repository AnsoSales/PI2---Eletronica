#define trigger GPIO_NUM_26
#define echo GPIO_NUM_25

const int numLecturas = 10;
float tempo,dist,media = 0,lecturas[numLecturas],volume=0;

const float Ainf=130,Asup=300; //ate 0.5L 72.25


int lecturaActual = 0; // Lectura por la que vamos
float total = 0; // Total de las que llevamos


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
  for (int i = 0; i < numLecturas; i++)
  {
    lecturas[i] = 0;
  }



}

void loop() {
  total = total - lecturas[lecturaActual];
  sonico(trigger,echo);
  dist= tempo*0.034/2;
  lecturas[lecturaActual] = dist;
  total = total + lecturas[lecturaActual];
  lecturaActual = lecturaActual + 1;
  if (lecturaActual >= numLecturas)
  {
    lecturaActual = 0;
  }
  media = total / numLecturas; 
  volume=(9.91-media)*(Ainf+Asup+sqrt(Ainf*Asup))/3000;
  Serial.print(media);
  Serial.println("cm");
  Serial.print(volume);
  Serial.println("L");
  
  delay(400);
}
