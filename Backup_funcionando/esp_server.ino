#include <WiFi.h>

#define ledvermelho 04  
#define ledverde 15     
#define ledamarelo 22   

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

  digitalWrite(ledverde, 1);
  digitalWrite(ledvermelho, 1);
  digitalWrite(ledamarelo, 1);
}

void loop()
{
  WiFiClient client = server.available();

  if (client)
  {
    Serial.println("Cliente conectado com sucesso");

    if (client.connected())
    {
      if (client.available())
      {
        String request = client.readStringUntil('\r'); 
        client.flush(); 

        if (request.indexOf("GET /?comando=pessoa_errada") >= 0)
        {
          Serial.println("Comando pessoa_errada detectado");
          digitalWrite(ledamarelo, 0); 
        }
        else if (request.indexOf("GET /?comando=apagar_led") >= 0)
        {
          Serial.println("Comando apagar_led detectado");
          digitalWrite(ledamarelo, 1); 
        }
        else if (request.startsWith("carbono:"))
        {
          int carbono_analog = request.substring(8,request.length()).toInt();
          Serial.println("Valor do sensor de carbono recebido no servidor:");
          Serial.println(carbono_analog);
          if (carbono_analog > 100)
          {
            Serial.println("Acendeu LED de carbono");
            digitalWrite(ledvermelho, 0);
          }
          else
          {
            digitalWrite(ledvermelho, 1);
            Serial.println("Apagou LED de carbono");
          }
        }
        else if (request.startsWith("microfone:"))
        {
          int microfone_analog = request.substring(10,request.length()).toInt(); 
          Serial.println("Valor do microfone recebido no servidor:");
          Serial.println(microfone_analog);
          if (microfone_analog > 3500)
          {
            Serial.println("Acendeu LED do microfone");
            digitalWrite(ledverde, 0);
          }
          else
          {
            digitalWrite(ledverde, 1);
            Serial.println("Apagou LED do microfone");
          }
        }

        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<!DOCTYPE HTML>");
        client.println("<html><body>ESP32 Server</body></html>");
        
        Serial.println("Cliente desconectado");
      }
    }
  }
}
