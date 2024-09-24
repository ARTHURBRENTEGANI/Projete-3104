#include <WiFi.h>
#include <WiFiClient.h>

const int senscarbono = 35; 

const char* nome = "Servidor3104";
const char* senha = "12345678";
const int porta = 600;
const IPAddress ip_servidor(192, 168, 4, 1);
String resposta_servidor;


void setup() 
{
  Serial.begin(115200);
  pinMode(senscarbono, INPUT);

WiFi.begin(nome,senha);

while(WiFi.status() != WL_CONNECTED)
{
Serial.println("Conectando ...");
delay(1000);  
}

Serial.println();
Serial.println("Conectado com sucesso a rede wifi");

Serial.println();
Serial.println("o ip da rede wifi do esp32 do carbono como cliente é: ");
Serial.print(WiFi.localIP());

}


void loop() 
{
  int sencarbono_analog = analogRead(senscarbono);

  Serial.print("Valor lido pelo sennsor de carbono: ");
  Serial.println(sencarbono_analog);

  delay(1000);

  WiFiClient client;

if(client.connect(ip_servidor,porta))//tenta conectar ao servidor
{
Serial.println();
Serial.println("conectado com sucesso ao servidor");

client.print("carbono:");
client.print(sencarbono_analog); // envia o valor do sensor para o servidor 
client.println(); // envia a linha em branco com /r para indicar o fim da mensagem, usado no protocolo HTTP

// resposta do servior
while (client.connected() && !client.available())  // verificas e o cliente esta conectado e se o servidor não mandou nenhum dado
{
  delay(1000); 
}
while(client.available())
{
resposta_servidor = client.readStringUntil('\r'); // le a resposta do servidor e salva na variavel 

}

client.stop();
Serial.println("Desconectado do servidor");

}

else
{
Serial.println("Falha ao tentar conectar com o servidor");
}
delay(3000);
}
