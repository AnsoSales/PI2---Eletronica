#define trigger GPIO_NUM_26
#define echo GPIO_NUM_25

float tempo,dist,media = 0;

//void IRAM_ATTR gpio_isr_handler_up(void* arg)
//{
//  
//  portYIELD_FROM_ISR();
//  }


void ultrassonico(gpio_num_t A, gpio_num_t B)
{
  gpio_set_direction(A, GPIO_MODE_OUTPUT);
  gpio_set_direction(B, GPIO_MODE_INPUT);
 // gpio_set_intr_type(B, GPIO_INTR_HIGH_LEVEL);
  //gpio_set_pull_mode(B, GPIO_PULLUP_ONLY);
 // gpio_intr_enable(B);
 // gpio_install_isr_service(0);
//  gpio_isr_handler_add(B, gpio_isr_handler_up, (void*) B);
//  timer = timerBegin(0, 80, true);
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
  // put your setup code here, to run once:
  
Serial.begin(115200);
ultrassonico(trigger,echo);

}

void loop() {
  // put your main code here, to run repeatedly:
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
