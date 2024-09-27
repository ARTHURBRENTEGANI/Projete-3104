#include <WiFi.h>
#include <WiFiClient.h>

const int senscarbono = 35; 
const char* nome = "Servidor3104";
const char* senha = "12345678";
const int porta = 600;
const IPAddress ip_servidor(192, 168, 4, 1);
String resposta_servidor;
unsigned long int tempo = 0;
unsigned long int intervalo = 4000;

void setup() 
{
  Serial.begin(115200);
  pinMode(senscarbono, INPUT);

  WiFi.begin(nome, senha);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Conectando ...");
    delay(1000);  
  }

  Serial.println();
  Serial.println("Conectado com sucesso a rede wifi");
  Serial.println();
  Serial.println("O IP da rede wifi do ESP32 do carbono como cliente é: ");
  Serial.print(WiFi.localIP());
}

void loop() 
{
  int sencarbono_analog = analogRead(senscarbono);

  Serial.print("Valor lido pelo sensor de carbono: ");
  Serial.println(sencarbono_analog);

  WiFiClient client;

  if (millis() - tempo >= intervalo) 
  {
    tempo = millis();  

    if (client.connect(ip_servidor, porta))
    {
      Serial.println();
      Serial.println("Conectado com sucesso ao servidor");

      client.print("carbono:");
      client.print(sencarbono_analog);
      client.println();

      while (client.connected() && !client.available())
      {
        delay(100);  
      }

      while (client.available())
      {
        resposta_servidor = client.readStringUntil('\r');
        Serial.println("Resposta do servidor: " + resposta_servidor);
      }

      client.stop();
      Serial.println("Desconectado do servidor");
    }
    else
    {
      Serial.println("Falha ao tentar conectar com o servidor");
    }
  }
}
