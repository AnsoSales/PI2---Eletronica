#define trigger GPIO_NUM_26
#define echo GPIO_NUM_25

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
 
Serial.begin(115200);
ultrassonico(trigger,echo);

}

void loop() {
  
  sonico(trigger,echo);
  
  dist= tempo*0.034/2;
  Serial.print("Distancia: ");
  for(int i = 0; i<9; i++)
  {
    media += dist/10;
  }
  Serial.println(media);
  delay(10);
  tempo = 0;
  media = 0;
}
