#include "DHT.h"
#define DHTPIN A0  
#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{ 
pinMode(3,OUTPUT); 
pinMode(2,OUTPUT); 
pinMode(A0, INPUT);
pinMode(A2, INPUT);
pinMode(5, OUTPUT);
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);
pinMode(10, OUTPUT);
pinMode(11, OUTPUT);
pinMode(12, OUTPUT);
pinMode(13, OUTPUT);
dht.begin();
Serial.begin(9600);
}
 
void loop() 
{ 
digitalWrite(8,HIGH);
digitalWrite(9,HIGH);
digitalWrite(10,HIGH);
digitalWrite(11,HIGH);
digitalWrite(12,HIGH);
digitalWrite(13,HIGH);
int temp;
float h = dht.readHumidity();
float t = dht.readTemperature();
float hic = dht.computeHeatIndex(t, h, false);
if(hic>27)
temp=255;
else if(hic>25)
temp=127;
else
temp=0;
analogWrite(3, temp);         
digitalWrite(2,LOW); 
int a = analogRead(A2);
if(a>512)
{a=255;}
else
{a=0;}
analogWrite(5, a);
Serial.println(a);
}
