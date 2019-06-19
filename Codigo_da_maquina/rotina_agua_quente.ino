// definir porta de leitura do sensor de Vazao
const int portaVazao = GPIO_NUM_35;
static void atualizaVazao();
volatile int pulsos_vazao = 0;
float vazao = 0, media = 0, x_litros = 1, tempo = 0;
int i = 0;

// interrupção
void IRAM_ATTR gpio_isr_handler_up(void* arg)
{
  pulsos_vazao++;
  portYIELD_FROM_ISR();
}


void iniciaVazao(gpio_num_t Port) {
  gpio_set_direction(Port, GPIO_MODE_INPUT);
  gpio_set_intr_type(Port, GPIO_INTR_NEGEDGE);
  gpio_set_pull_mode(Port, GPIO_PULLUP_ONLY);
  gpio_intr_enable(Port);
  gpio_install_isr_service(0);
  gpio_isr_handler_add(Port, gpio_isr_handler_up, (void*) Port);
  cli(); //desativa a interrupção
}

void rotina_agua_quente()
{
  iniciaVazao((gpio_num_t) valvula2);
  enche_recipiente(); //enche o recipiente de acordo com a vazao eo volume requerido
}

void enche_recipiente()
{
  float media_x = 0;
  digitalWrite(valvula2, HIGH);  //libera a solenoide
  media_x = calibra();
  tempo = ((x_litros / media_x) * 1000); //tempo necessario para encher recipiente com x_litros em mili segundos
  delay(tempo - 3500); //descontando tempo de calibração
  digitalWrite(valvula2, LOW);
}


float calibra()
{
  media=0;
  while (1)
  {
    pulsos_vazao = 0;
    sei();
    delay(1000);
    cli();

    vazao = pulsos_vazao / 5.5;
    media = media + vazao;
    i++;
    if (i == 3)
    {
      media = media / 240;                    //faz a média retornando em Litros por Segundo, divindo a vazao em L/min por 60*(4 interaçoes da media)
      i = 0;                                 //Zera a variável i para uma nova contagem
      break;
    }
  }
  return media;
}

void setup()
{
  Serial.begin(115200);
}


void loop()
{
  rotina_agua_quente();
}
