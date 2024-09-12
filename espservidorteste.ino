#include <WiFi.h>

#define ledvermelho 26
#define ledverde 12
#define ledamarelo 13

String resposta_cliente;
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
pinMode(ledamarelo, OUTPUT);

digitalWrite(ledverde,0);
digitalWrite(ledvermelho,0);
digitalWrite(ledamarelo,0);
}

void loop()
{

 WiFiClient client = server.available();// a função server.available() verifica se tem um cliente tentando conectar e se tiver retorna um valor WiFiClient e se não estiver um cliente 
 //tentando se conectar retorna um WiFiClient inválido, se a função server.available() retorna um objeto válido a variavel client tera umvalor válido 
 
if (client != WiFiClient()) // verifica se avariavel client não é inválida
{
Serial.println("Existe um clliente conectado com sucesso");

if(client.connected()) //enquanto o cliente estiver conectado
 {
if(client.available()) // enquanto o cliente estiver mandando alguma coisa
  {
resposta_cliente = client.readStringUntil('\r'); // le a resposta enviada pelo cliente
if(resposta_cliente.length() > 0)
    {
Serial.println("Cliente mandou alguma coisa: ");
Serial.println(resposta_cliente);
if (resposta_cliente.startsWith("carbono:"))
{

int carbono_analog = resposta_cliente.substring(8,resposta_cliente.length()-1).toInt();
Serial.println("o valor do sensnro de carbono chedao no servidor é: ");
Serial.println(carbono_analog);
if(carbono_analog > 40)
{
Serial.println("acendeu led carbono");
digitalWrite(ledvermelho,1);
}
 else
{
digitalWrite(ledvermelho,0);
Serial.println("apagou led carbono");
}

}
else if(resposta_cliente.startsWith("microfone:"))
{
int microfone_analog =resposta_cliente.substring(10,resposta_cliente.length() - 1).toInt(); 
Serial.println("o valor do microfone que chegou no servidor é: ");
Serial.println(microfone_analog);
if(microfone_analog > 200)
{

Serial.println("acendeu led microfone");
digitalWrite(ledverde,1);
}
else
{
digitalWrite(ledverde,0);
Serial.println("apagou led microfone");
}
}
client.stop(); // desconecta o cliente
Serial.println("Cliente desconectado");

}
else
{
bool x;
while(x == 1)
{
Serial.println("a resposta do cliente esta vazia código esta travado dentro do bloco while");
}

}
  }

 }
}
else
{
Serial.println("não tem nehum cliente tentando se conectar");
}
delay(5000);
resposta_cliente = " ";
}
