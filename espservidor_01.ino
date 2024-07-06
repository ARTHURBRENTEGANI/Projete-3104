#include <WiFi.h>

#define ledvermelho 26
#define ledverde 12


void setup() 
{


pinMode(ledvermelho, OUTPUT);
pinMode(ledverde, OUTPUT);


}

void loop()
{
digitalWrite(ledverde,1);
digitalWrite(ledvermelho,1);
}
