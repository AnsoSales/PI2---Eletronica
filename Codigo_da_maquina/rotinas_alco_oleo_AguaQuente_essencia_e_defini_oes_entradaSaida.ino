
void setup()
{
    
}

void configura_bomba_solenoide()
{
    gpio_set_direction(bomba1  , GPIO_MODE_OUTPUT);//configura bomba1 p/ BOMBA Da balança
    digitalWrite(bomba1, LOW);
    gpio_set_direction(bomba2  , GPIO_MODE_OUTPUT);//configura bomba2 p/ BOMBA Do oleo 
    digitalWrite(bomba2, LOW);
    gpio_set_direction(bomba3 , GPIO_MODE_OUTPUT);//configura bomba3 p/ BOMBA Do alcool
    digitalWrite(bomba3, LOW);
    gpio_set_direction(bomba4 , GPIO_MODE_OUTPUT);//configura bomba4 p/ BOMBA Da essencia1
    digitalWrite(bomba4, LOW);
    gpio_set_direction(bomba5 , GPIO_MODE_OUTPUT);//configura bomba4 p/ BOMBA Da essencia2
    digitalWrite(bomba5, LOW);
    gpio_set_direction(bomba6 , GPIO_MODE_OUTPUT);//configura bomba4 p/ BOMBA Da agua quente
    digitalWrite(bomba6, LOW);

    gpio_set_direction(valvula1 , GPIO_MODE_OUTPUT);//configura valvula p/  VALVULA Da balança
    digitalWrite(valvula1, LOW);
    gpio_set_direction(valvula2 , GPIO_MODE_OUTPUT);//configura valvula p/  VALVULA Da agua quente
    digitalWrite(valvula2, LOW);
    

    gpio_set_direction(motor , GPIO_MODE_OUTPUT);//configura bomba4 p/ BOMBA Da agua quente
    digitalWrite(motor, LOW);

      
}

void rotina_oleo()
{
    digitalWrite(bomba2, HIGH);
    delay(tempo_oleo);             //tempo em milisegundos necessario para escoar o volume definido
    digitalWrite(bomba2, LOW); 
  
}

void rotina_alcool()
{
    digitalWrite(bomba3, HIGH);
    delay(tempo_alcool);             //tempo em milisegundos necessario para escoar o volume definido
    digitalWrite(bomba3, LOW); 
}

void rotina_essencia_1()
{
    digitalWrite(bomba4, HIGH);
    delay(tempo_alcool);             //tempo em milisegundos necessario para escoar o volume definido
    digitalWrite(bomba4, LOW); 
}

void rotina_essencia_2()
{
    digitalWrite(bomba5, HIGH);
    delay(tempo_alcool);             //tempo em milisegundos necessario para escoar o volume definido
    digitalWrite(bomba5, LOW); 
}

void rotina_agua_quente()
{
    digitalWrite(bomba6, HIGH);
    delay(tempo_alcool);             //tempo em milisegundos necessario para escoar o volume definido
    digitalWrite(bomba6, LOW); 
}

void rotina_motor()
{
    digitalWrite(motor, HIGH);
    delay(tempo_alcool);             //tempo em milisegundos necessario para escoar o volume definido
    digitalWrite(motor, LOW); 
}





void loop() {
   
   }
