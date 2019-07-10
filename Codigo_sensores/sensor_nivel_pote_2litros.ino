#define PinTrig GPIO_NUM_26   
#define PinEcho GPIO_NUM_25

 

const float VelSon = 34000.0;

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
 
 
  unsigned long tiempo = pulseIn(PinEcho, HIGH);
 

  float distancia = tiempo * 0.000001 * VelSon / 2.0;
 

  lecturas[lecturaActual] = distancia;
 

  total = total + lecturas[lecturaActual];
 

  lecturaActual = lecturaActual + 1;
 

  if (lecturaActual >= numLecturas)
  {
    primeraMedia = true;
    lecturaActual = 0;
  }
 

  media = total / numLecturas;
     
	

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
