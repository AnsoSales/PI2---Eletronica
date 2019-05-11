const int porta_A = GPIO_NUM_35;
const float constant_sensor=7.5;
volatile int pulsos = 0;
float vazao = 0;


/*
O sensor de fluxo envia pulsos ao microcontrolador, e para a obtenção destes pulsos é necessário
usar interrupçoes para captar as bordas de subiddas destes pulsos.
*/

//Esta interrupçao é ativda quando ocorre uma borda de subida na entrada da porta A
void IRAM_ATTR gpio_interrupt_rissing(void* arg) //aqui definimos qual vai ser nossa funçao de interrupçao
{
  pulsos++;
  portYIELD_FROM_ISR(); //prioeidade da tarefa (xHigherPriorityTaskWoken)
}

/*
conta as bordas de subida da porta que iremos analisar a partir de uma interrupçao com borda de subida
 */
void obter_vazao(gpio_num_t porta_A){
  gpio_set_direction(porta_A, GPIO_MODE_INPUT);        // Argumentos (Qual a GPIO gpio_num_x,e o modo GPIO_mode_x) : modos= output_only,input_only,output_and_input
  gpio_set_intr_type(porta_A, GPIO_INTR_NEGEDGE);      // GPIO set interrupt trigger type | gpio_num_X ,  intr_type_NEGEDGE OR  intr_type_POSEDGE
  gpio_set_pull_mode(porta_A, GPIO_PULLUP_ONLY);       // User this Function,configure GPIO pull mode,such as pull-up,pull-down |  gpio_num_X, GPIO_X  X:=pullup_only,pulldown_only,pullup_and_pulldown
  gpio_intr_enable(porta_A);                           // enable GPIO module interrupt signal |   gpio_num_X
  gpio_install_isr_service(0);                                                //  instalar o servio que gerencia as interrupções para os pinos de GPIO:0
  gpio_isr_handler_add(porta_A, gpio_interrupt_rissing, (void*) porta_A);        //  especificar para cada pino em qual núcleo sua ISR (Interrupt Service Routine) será executado parametros(pinos,funçãoParaInterrup,ArrayDargumentos) (gpio_num_tgpio_num, gpio_isr_tisr_handler, void *args)
}

void setup() {
  
  Serial.begin(115200);
  obter_vazao((gpio_num_t) porta_A);
}

void loop() {
  vazao = pulsos/constant_sensor;
  pulsos = 0;


  Serial.println("Vazao L/m:");
  Serial.println(vazao,4);
  delay(1000);

}
