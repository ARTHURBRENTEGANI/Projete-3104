#include <WiFi.h>

#define ledvermelho 32
#define ledverde 35
#define ledamarelo 34

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
    // O IP do ESP é 192.168.4.1
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
        resposta_cliente = client.readStringUntil('\r'); // Lê a resposta enviada pelo cliente
        if (resposta_cliente.length() > 0)
        {
          Serial.println("Cliente mandou alguma coisa:");
          Serial.println(resposta_cliente);

          if (resposta_cliente.startsWith("carbono:"))
          {
            int carbono_analog = resposta_cliente.substring(8, resposta_cliente.length() - 1).toInt();
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
          else if (resposta_cliente.startsWith("microfone:"))
          {
            int microfone_analog = resposta_cliente.substring(10, resposta_cliente.length() - 1).toInt();
            Serial.println("Valor do microfone recebido no servidor:");
            Serial.println(microfone_analog);
            if (microfone_analog > 200)
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
          else if (resposta_cliente.startsWith("pessoa_errada"))
          {
            Serial.println("Classe person_wrong detectada");
            digitalWrite(ledamarelo, HIGH); // Acende o LED amarelo
          }
          else
          {
            digitalWrite(ledamarelo, LOW); // Apaga o LED amarelo se não for a classe esperada
          }
          client.stop(); // Desconecta o cliente
          Serial.println("Cliente desconectado");
        }
      }
    }
  }
  else
  {
    Serial.println("Nenhum cliente tentando se conectar");
  }
  delay(5000);
  resposta_cliente = " ";
}
