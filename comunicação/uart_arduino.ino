int led = 2;
int comando = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(2,OUTPUT);

}

void loop() {
  if (Serial.available()>0)
  {
    comando = Serial.read();

        if (comando == '1')
          {
            digitalWrite(led,HIGH);
            Serial.println("esquentando");
            delay(5000);
            Serial.println("concluido");
            digitalWrite(led,LOW);
          }
  }
  // put your main code here, to run repeatedly:

}
