#include <WiFi.h>
#include <WiFiClient.h>

const int senscarbono = 35; 
const char* nome = "Servidor3104";
const char* senha = "12345678";
const int porta = 600;
const IPAddress ip_servidor(192, 168, 4, 1);
String resposta_servidor;
unsigned long int tempo = 0;
unsigned long int intervalo = 6000;  // Intervalo de 6 segundos

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

  // Verifica se o intervalo passou para enviar nova leitura
  if (millis() - tempo >= intervalo) 
  {
    // Atualiza o tempo para o próximo envio
    tempo = millis();  

    if (client.connect(ip_servidor, porta)) // Tenta conectar ao servidor
    {
      Serial.println();
      Serial.println("Conectado com sucesso ao servidor");

      // Envia valor do sensor para o servidor
      client.print("carbono:");
      client.print(sencarbono_analog);
      client.println(); // Finaliza a mensagem com /r

      // Aguarda resposta do servidor
      while (client.connected() && !client.available())
      {
        delay(100);  // Pequeno delay para evitar loop vazio
      }

      // Lê a resposta do servidor
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
  
  // Outras lógicas podem ser colocadas aqui sem interromper o tempo do millis()
}
