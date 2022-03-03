#define BLYNK_MAX_SENDBYTES 256
#define BLYNK_PRINT Serial  
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Servo.h>
Servo myservo;

int n=1;
int pos;
char auth[] = "xmvSJLHOiSZgD0daAGU5NdDBEviUfIgl";


char numeWIFI[] = "DIGI-vzZ9";
char parola[] = "MUmczYtA"; 

#define DHTPIN 5          // Digital pin 1

#define DHTTYPE DHT11     // DHT 11


DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
boolean flagTempMax=true;
boolean flagTempMin=true;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); //celsius

  if (isnan(h) || isnan(t)) {
    Serial.println("Nu s-a citit de pe senzor!");
    return;
  }
  Serial.println(t);
  //poti trimite orice valoare
  Blynk.virtualWrite(V5, h);  //V5 pentru umiditate
  Blynk.virtualWrite(V6, t);  //V6 pentru temperatura
  
  if(t > 27 && flagTempMax == true){
     Blynk.email("iulia_nicorici20@yahoo.com", "ESP8266 Alert", "Temperatura este peste 27!Se va porni aerul contionat");
     Blynk.notify("Temperatura este peste 27!Se va porni aerul contionat");
     Serial.println("Temperatura este peste 27!Se va porni aerul contionat");
     flagTempMax = false;
     myservo.write(0);
     Blynk.virtualWrite(V7, 0);
     delay(5000);
  }
  if(t < 20 && flagTempMin == true){
     Blynk.email("iulia_nicorici20@yahoo.com", "ESP8266 Alert", "Temperatura este sub 20 de grade !Se va porni centrala");
     Blynk.notify("Temperatura este sub 20 de grade !Se va porni centrala");
     Serial.println("Temperatura este sub 20 de grade !Se va porni centrala");
     flagTempMin = false;
     myservo.detach(); 
     delay(5000);
  }
  if(t > 27){
     n++;
     myservo.write(n*2);
     Blynk.virtualWrite(V7, n*2);
     if(n*2 >= 180){
        n=1;
        delay(20);
     }
  }
  if(t < 27 && flagTempMax == false){
    flagTempMax = true;
  }
  if(t < 20 && flagTempMin == false){
    flagTempMin = true;
  }
  
}

void setup()
{
  Serial.begin(9600); // Serial Monitor
  Blynk.begin(auth, numeWIFI, parola);

  dht.begin();
  myservo.attach(2); //sevo motor pin 4
  // functie chemata in fiecare secunda
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run(); // Initiaza Blynk
  timer.run(); // Initiaza BlynkTimer
} 
