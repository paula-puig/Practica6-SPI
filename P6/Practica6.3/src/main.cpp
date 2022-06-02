#include <Arduino.h>
#include <Wire.h>
#include "RTClib.h"
#include<SPI.h>

RTC_DS1307 rtc;

int segundo,minuto,hora,dia,mes;
long anio; //variable año
DateTime HoraFecha;

void setup () {
  Serial.begin(115200);
  rtc.begin(); //Inicializamos el RTC
  
}

void loop () {
    HoraFecha = rtc.now(); //obtenemos la hora y fecha actual
    
    segundo=HoraFecha.second();
    minuto=HoraFecha.minute();
    hora=HoraFecha.hour();
    dia=HoraFecha.day();
    mes=HoraFecha.month();
    anio=HoraFecha.year();

    //Enviamos por el puerto serie la hora y fecha.
    Serial.print("hora = ");
    Serial.print(hora);
    Serial.print(":");
    Serial.print(minuto);
    Serial.print(":");
    Serial.print(segundo);
    Serial.print("  Fecha = ");
    Serial.print(dia);
    Serial.print("/");
    Serial.print(mes);
    Serial.print("/");
    Serial.print(anio);
    Serial.println();
    delay(1000);
}
 
