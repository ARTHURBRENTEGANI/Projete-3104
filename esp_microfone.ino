#include <WiFi.h>
#include <WiFiClient.h>

const int mic = 34; 

const char* nome = "Servidor3104";
const char* senha = "12345678";
const int porta = 600;
const IPAddress ip_servidor(192, 168, 4, 1);
String resposta_servidor;

void setup() 
{

  Serial.begin(115200);
  pinMode(mic, INPUT);

WiFi.begin(nome,senha);

while(WiFi.status() != WL_CONNECTED)
{
Serial.println("Conectando ...");
delay(1000);  
}

Serial.println();
Serial.println("Conectado com sucesso a rede wifi");

Serial.println();
Serial.println("o ip da rede wifi do esp32 com o microfone como cliente é: ");
Serial.print(WiFi.localIP());


}

void loop() 
{

  int mic_analo = analogRead(mic);
  Serial.println(mic_analo);

  WiFiClient client;

if(client.connect(ip_servidor,porta))//tenta conectar ao servidor
{
Serial.println();
Serial.println("conectado com sucesso ao servidor");

client.print("microfone:");

if(mic_analo < 700 || mic_analo > 3200)
{
client.print(mic_analo);
}
else
{
bool x = 1;
while(x == 1 )
{
Serial.println("O valor do microfone não foi suficiente ");
delay(5000);
}
} 
client.println(); // envia a linha em branco com /r para indicar o fim da mensagem, usado no protocolo HTTP

// resposta do servior
while (client.connected() && !client.available())  // verificas e o cliente esta conectado e se o servidor não mandou nenhum dado
{
  delay(1000); 
}
while(client.available())
{
resposta_servidor = client.readStringUntil('\r'); // le a resposta do servidor e salva na variavel 
Serial.println("A resposta do servidor é: ");
Serial.println(resposta_servidor);
}
client.stop();
Serial.println("Desconectado do servidor");

}

else
{
Serial.println("Falha ao tentar conectar com o servidor");
}
delay(4000);
}
