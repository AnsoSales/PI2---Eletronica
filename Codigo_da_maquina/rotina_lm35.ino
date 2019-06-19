

void temperatura_tank()
{
  int temp_digital= 0;
  double Voltage = 0;
  double tempC = 0,media=0;
  
  for(int i = 0; i<10; i++)
  {
    temp_digital = analogRead(lmdata);   //leitura da gpio 35 da esp32
    media += temp_digital/10;
  }
    Voltage = (media / 2048.0) * 3300;
    tempC = Voltage * 0.1; 
    media=0;

  
}


 
void setup(){
Serial.begin(9600);
}
 
void loop(){

temperatura_tank();
delay(1000);
}
