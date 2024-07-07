#include <WiFi.h>

#define ledvermelho 26
#define ledverde 12


const char* nome = "Servidor3104";
const char* senha = "12345678";

WiFiServer server(600);

void setup() 
{
Serial.begin(115200);

if(WiFi.softAP(nome,senha))
{
Serial.println("rede Wifi gerada com sucesso !!!!");
Serial.println("o ip do servidor do esp servidor é :");
Serial.println(WiFi.softAPIP());
// o ip do esp é 192.168.4.1
}
else
{
Serial.println("erro ao gerar a rede wifi");  
}

server.begin();

pinMode(ledvermelho, OUTPUT);
pinMode(ledverde, OUTPUT);

digitalWrite(ledverde,0);
digitalWrite(ledvermelho,0);

}

void loop()
{
digitalWrite(ledverde,1);
digitalWrite(ledvermelho,1);
}
