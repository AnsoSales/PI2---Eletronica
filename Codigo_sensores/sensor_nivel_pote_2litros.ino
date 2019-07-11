#define PinTrig GPIO_NUM_26   
#define PinEcho GPIO_NUM_25

 

const float VelSom = 34000.0;

const float Ainf=136.89,Asup=338.56; //ate 0.5L 72.25
 


 
float h=0,volume=0; 
; 
float total = 0; 
float media = 0;

void setup()
{
	
  Serial.begin(9600);

  pinMode(PinTrig, OUTPUT);

  pinMode(PinEcho, INPUT);
 
  

}
void loop()
{


 
  iniciarTrigger();
 
 
  unsigned long tempo = pulseIn(PinEcho, HIGH);
 

  float distancia = tempo * 0.000001 * VelSom/ 2.0;
 

  leituras[leituraAtual] = distancia;
 

  total = total + leituras[leituraAtual];
 

  leituraActual = leituraAtual + 1;
 

  if (leituraAtual >= numLeituras)
  {
    primeraMedia = true;
    leituraAtual = 0;
  }
 

  media = total / numLeituras;
     
	

  if (primeraMedia)
  {
    Serial.print(media);
    Serial.println("cm");
    Serial.print(volume,4);
    Serial.println("L");  
  }



  delay(500);
}
 

void iniciarTrigger()
{

  digitalWrite(PinTrig, LOW);
  delayMicroseconds(2);

  digitalWrite(PinTrig, HIGH);
  delayMicroseconds(10);
 

  digitalWrite(PinTrig, LOW);
}
