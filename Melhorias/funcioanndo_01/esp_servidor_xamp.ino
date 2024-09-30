#include <WiFi.h>
#include <HTTPClient.h>  // Biblioteca para fazer requisições HTTP

#define ledvermelho 4  // Pino para LED vermelho (sensor de carbono)
#define ledverde 15     // Pino para LED verde (sensor de microfone)
#define ledamarelo 22   // Pino para LED amarelo (detecção de pessoa errada)

String resposta_cliente;
const char* nome = "Servidor3104"; // Nome da rede Wi-Fi
const char* senha = "12345678";    // Senha da rede Wi-Fi

WiFiServer server(600); // Cria o servidor na porta 600

void envia_sql(int red, int green, int yellow) 
{
Serial.print("Enviando os leds: ");
Serial.print("Red: ");
Serial.print(red);
Serial.print(", Green: "); 
Serial.print(green);
Serial.print(", Yellow: ");
Serial.println(yellow);

  
 HTTPClient http;
String serverPath = "http://192.168.4.2/estado_led.php?red=" + String(red) + "&green=" + String(green) + "&yellow=" + String(yellow);
Serial.println("caminho da requisição http");
Serial.println(serverPath);
    
 http.begin(serverPath.c_str());
    
int resposta_http = http.GET();
Serial.println("resposta obtida pelo http");
Serial.println(resposta_http);
    
    if (resposta_http > 0) 
    {
      String resposta_string = http.getString();  // Lê a resposta
      Serial.println("Resposta do servidor SQL");
      Serial.println(resposta_string);             // Resposta do servidor
    
     if (resposta_string.indexOf("Dados inseridos com sucesso!") >= 0) 
     {
        Serial.println("Dados enviados com sucesso ao servidor.");
      } else 
      {
        Serial.println("Falha ao enviar dados: " + resposta_string);
      }
    
    }
    else {
      Serial.print("Erro na requisição: ");
      Serial.println(resposta_http);
    }
    
    http.end();
  
  }

void setup() {
  Serial.begin(115200); 
  if (WiFi.softAP(nome, senha)) 
  {
    Serial.println("Rede WiFi gerada com sucesso !!!!");
  }
  else {
    Serial.println("Erro ao gerar a rede WiFi");  
  }

  server.begin(); // Inicia o servidor

  // Define os pinos dos LEDs como saída
  pinMode(ledvermelho, OUTPUT);
  pinMode(ledverde, OUTPUT);
  pinMode(ledamarelo, OUTPUT);

  // Inicializa os LEDs como apagados
  digitalWrite(ledverde, 1);
  digitalWrite(ledvermelho, 1);
  digitalWrite(ledamarelo, 1);
}

void loop() {
  WiFiClient client = server.available(); // Verifica se há um cliente tentando conectar

  if (client) {
    Serial.println("Cliente conectado com sucesso");

    if (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r'); // Lê a requisição completa
        client.flush(); // Limpa o buffer

        // Verifica os comandos recebidos
        if (request.indexOf("GET /?comando=pessoa_errada") >= 0) {
          Serial.println("Comando pessoa_errada detectado");
          digitalWrite(ledamarelo, 0); // Acende o LED amarelo
     envia_sql(digitalRead(ledvermelho),digitalRead(ledverde), digitalRead(ledamarelo));
        }
        else if (request.indexOf("GET /?comando=apagar_led") >= 0) {
          Serial.println("Comando apagar_led detectado");
          digitalWrite(ledamarelo, 1); // Apaga o LED amarelo
   envia_sql(digitalRead(ledvermelho),digitalRead(ledverde), digitalRead(ledamarelo));
        }
        else if (request.startsWith("carbono:")) {
          int carbono_analog = request.substring(8, request.length()).toInt();
          Serial.println("Valor do sensor de carbono recebido no servidor:");
          Serial.println(carbono_analog);
          if (carbono_analog > 100) {
            Serial.println("Acendeu LED de carbono");
            digitalWrite(ledvermelho, 0); // Acende o LED vermelho
            envia_sql(digitalRead(ledvermelho),digitalRead(ledverde), digitalRead(ledamarelo));  // Envia dados para o banco
          }
          else {
            digitalWrite(ledvermelho, 1); // Apaga o LED vermelho
            Serial.println("Apagou LED de carbono");
       envia_sql(digitalRead(ledvermelho),digitalRead(ledverde), digitalRead(ledamarelo));
          }
        }
        else if (request.startsWith("microfone:")) {
          int microfone_analog = request.substring(10, request.length()).toInt();
          Serial.println("Valor do microfone recebido no servidor:");
          Serial.println(microfone_analog);
          if (microfone_analog > 2000) 
          {
            Serial.println("Acendeu LED do microfone");
            digitalWrite(ledverde, 0); // Acende o LED verde
     envia_sql(digitalRead(ledvermelho),digitalRead(ledverde), digitalRead(ledamarelo));
          }
          else {
            digitalWrite(ledverde, 1); // Apaga o LED verde
            Serial.println("Apagou LED do microfone");
           envia_sql(digitalRead(ledvermelho),digitalRead(ledverde), digitalRead(ledamarelo));
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
