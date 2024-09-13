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

    if (WiFi.softAP(nome, senha)) {
        Serial.println("Rede Wifi gerada com sucesso!");
        Serial.println("O IP do servidor ESP32 é:");
        Serial.println(WiFi.softAPIP());
        // O IP do ESP32 é 192.168.4.1
    } else {
        Serial.println("Erro ao gerar a rede Wifi.");
    }

    server.begin();

    pinMode(ledvermelho, OUTPUT);
    pinMode(ledverde, OUTPUT);
    pinMode(ledamarelo, OUTPUT);

    digitalWrite(ledverde, 0);
    digitalWrite(ledvermelho, 0);
    digitalWrite(ledamarelo, 0);
}

void loop() 
{
    WiFiClient client = server.available(); // Verifica se há um cliente tentando conectar
    
    if (client) {
        Serial.println("Cliente conectado com sucesso");

        if (client.connected()) {
            if (client.available()) {
                resposta_cliente = client.readStringUntil('\r'); // Lê a resposta enviada pelo cliente
                
                if (resposta_cliente.length() > 0) {
                    Serial.println("Cliente mandou alguma coisa:");
                    Serial.println(resposta_cliente);

                    // Lógica para os dados de sensores existentes
                    if (resposta_cliente.startsWith("carbono:")) {
                        int carbono_analog = resposta_cliente.substring(8, resposta_cliente.length() - 1).toInt();
                        Serial.println("O valor do sensor de carbono é:");
                        Serial.println(carbono_analog);

                        if (carbono_analog > 40) {
                            Serial.println("Acendeu o LED vermelho (carbono)");
                            digitalWrite(ledvermelho, HIGH);
                        } else {
                            digitalWrite(ledvermelho, LOW);
                            Serial.println("Apagou o LED vermelho (carbono)");
                        }
                    } else if (resposta_cliente.startsWith("microfone:")) {
                        int microfone_analog = resposta_cliente.substring(10, resposta_cliente.length() - 1).toInt();
                        Serial.println("O valor do microfone é:");
                        Serial.println(microfone_analog);

                        if (microfone_analog > 200) {
                            Serial.println("Acendeu o LED verde (microfone)");
                            digitalWrite(ledverde, HIGH);
                        } else {
                            digitalWrite(ledverde, LOW);
                            Serial.println("Apagou o LED verde (microfone)");
                        }
                    }

                    // Lógica para a classe "person_wrong"
                    else if (resposta_cliente.indexOf("led=amarelo") != -1) {
                        Serial.println("Classe 'person_wrong' detectada. Acendendo o LED amarelo.");
                        digitalWrite(ledamarelo, HIGH);
                        delay(5000);  // Mantém o LED amarelo aceso por 5 segundos
                        digitalWrite(ledamarelo, LOW);
                        Serial.println("Apagou o LED amarelo.");
                    }

                    client.stop(); // Desconecta o cliente
                    Serial.println("Cliente desconectado");
                }
            }
        }
    } else {
        Serial.println("Nenhum cliente tentando se conectar");
    }

    delay(5000); // Aguarda 5 segundos antes de verificar novamente
}
