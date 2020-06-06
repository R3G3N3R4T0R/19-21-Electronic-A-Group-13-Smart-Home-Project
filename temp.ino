#include "DHT.h"
#define DHTPIN A0  
#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{ 
pinMode(3,OUTPUT); 
pinMode(2,OUTPUT); 
pinMode(A0, INPUT);
pinMode(A1, INPUT);
pinMode(5, OUTPUT);
dht.begin();
Serial.begin(9600);
}
 
void loop() 
{ 
int temp;
float h = dht.readHumidity();
float t = dht.readTemperature();
float hic = dht.computeHeatIndex(t, h, false);
Serial.print(hic);
if(hic>27)
temp=255;
else if(hic>25)
temp=127;
else
temp=0;
analogWrite(3, temp);         
digitalWrite(2,LOW); 
int a = analogRead(A1);
if(a>512)
{a=255;}
else
{a=0;}
analogWrite(5, a);
}
