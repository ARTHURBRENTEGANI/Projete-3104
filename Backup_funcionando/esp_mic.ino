#include <WiFi.h>
#include <WiFiClient.h>

WiFiClient client;
const int mic = 35; 

const char* nome = "Servidor3104";
const char* senha = "12345678";
const int porta = 600;
const IPAddress ip_servidor(192, 168, 4, 1);
String resposta_servidor;

unsigned long int tempo = 0;

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(nome, senha);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Conectando ...");
    delay(1000);  
  }

  Serial.println();
  Serial.println("Conectado com sucesso a rede wifi");
}

void loop() 
{
  int mic_analo = analogRead(mic);
  Serial.println(mic_analo);
  Serial.println("valor do mic logo abaixo");
  Serial.println(mic_analo);

  if (client.connect(ip_servidor, porta))
  {
    Serial.println("conectou .....");

    if ((millis() - tempo) >= 2000)
    {
      client.print("microfone:");
      client.print(mic_analo);
      client.println();
      Serial.println("valor do mic no Milis");
      Serial.println(mic_analo);
      client.stop();
      Serial.println("passou no milis");
    }
  }

  delay(3000);
}
