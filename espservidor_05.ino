#include <WiFi.h>

#define ledvermelho 32  // Pino para LED vermelho (sensor de carbono)
#define ledverde 33     // Pino para LED verde (sensor de microfone)
#define ledamarelo 27   // Pino para LED amarelo (detecção de pessoa errada)

String resposta_cliente;
const char* nome = "Servidor3104";
const char* senha = "12345678";

WiFiServer server(600);

void setup() 
{
  Serial.begin(115200);

  if (WiFi.softAP(nome, senha))
  {
    Serial.println("Rede WiFi gerada com sucesso !!!!");
    Serial.println("O IP do servidor do ESP é:");
    Serial.println(WiFi.softAPIP());
  }
  else
  {
    Serial.println("Erro ao gerar a rede WiFi");  
  }

  server.begin();

  pinMode(ledvermelho, OUTPUT);
  pinMode(ledverde, OUTPUT);
  pinMode(ledamarelo, OUTPUT);

  digitalWrite(ledverde, LOW);
  digitalWrite(ledvermelho, LOW);
  digitalWrite(ledamarelo, LOW);
}

void loop()
{
  WiFiClient client = server.available(); // Verifica se há um cliente tentando conectar

  if (client)
  {
    Serial.println("Cliente conectado com sucesso");

    if (client.connected())
    {
      if (client.available())
      {
        String request = client.readStringUntil('\r'); // Lê a requisição completa
        client.flush(); // Limpa o buffer

        Serial.println("Cliente mandou alguma coisa:");
        Serial.println(request); // Imprime a requisição completa

        // Verifica se a requisição contém o comando esperado para cada sensor
        if (request.indexOf("GET /?comando=pessoa_errada") >= 0)
        {
          Serial.println("Comando pessoa_errada detectado");
          digitalWrite(ledamarelo, HIGH); // Acende o LED amarelo
        }
        else if (request.indexOf("GET /?comando=apagar_led") >= 0)
        {
          Serial.println("Comando apagar_led detectado");
          digitalWrite(ledamarelo, LOW); // Apaga o LED amarelo
        }
        else if (request.startsWith("carbono:"))
        {
          int carbono_analog = request.substring(8,request.length()).toInt();
          Serial.println("Valor do sensor de carbono recebido no servidor:");
          Serial.println(carbono_analog);
          if (carbono_analog > 40)
          {
            Serial.println("Acendeu LED de carbono");
            digitalWrite(ledvermelho, HIGH);
          }
          else
          {
            digitalWrite(ledvermelho, LOW);
            Serial.println("Apagou LED de carbono");
          }
        }
        else if (request.startsWith("microfone:"))
        {
          int microfone_analog =request.substring(10,request.length()).toInt(); 
          Serial.println("Valor do microfone recebido no servidor:");
          Serial.println(microfone_analog);
          if (microfone_analog > 1000)  // Ajuste conforme necessário
          {
            Serial.println("Acendeu LED do microfone");
            digitalWrite(ledverde, HIGH);
          }
          else
          {
            digitalWrite(ledverde, LOW);
            Serial.println("Apagou LED do microfone");
          }
        }

        // Responde ao cliente com uma mensagem HTTP 200 OK
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<!DOCTYPE HTML>");
        client.println("<html><body>ESP32 Server</body></html>");
        
        client.stop(); // Desconecta o cliente
        Serial.println("Cliente desconectado");
      }
    }
  }
  else
  {
    Serial.println("Nenhum cliente tentando se conectar");
  }
  delay(5000);
}
