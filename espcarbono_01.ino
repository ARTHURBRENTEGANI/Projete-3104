
const int senscarbono = 35; 
void setup() 
{
  Serial.begin(9600);
  pinMode(senscarbono, INPUT);
}

void loop() 
{
  int sencarbono_analog = analogRead(senscarbono);

  Serial.print("Valor lido pelo sennsor de carbono: ");
  Serial.println(sencarbono_analog);

  delay(1000);
}
