# **PRÀCTICA 6: BUSOS DE COMUNICACIÓ SPI**
## **PART A: Lectura SD**
### **6.1 Codi**

```c++
    #include <SPI.h>  
    #include <SD.h>  
    File myFile;  
    void setup(){  
        Serial.begin(115200);  
        Serial.print("Iniciando SD ...");  
        if (!SD.begin(4)) {  
            Serial.println("No se pudo inicializar");  
            return;  
        }  
        Serial.println("inicializacion exitosa");  

        myFile = SD.open("archivo.txt");//abrimos el archivo  
        if (myFile) {  
            Serial.println("archivo.txt:");  
            while (myFile.available()) {  
                Serial.write(myFile.read());  
            }  
            myFile.close(); //cerramos el archivo  
        } else {  
            Serial.println("Error al abrir el archivo");  
        }  
    }  
    void loop() {  
    } 
``` 

### **6.2 Funcionament**
El codi permet que la esp32 mitjançant un periferic SPI com ara es la targeta SD fagi una lectura de la targeta SD. S'hautra de crear l'arxiu "archivo.txt" que sera el que obtindrem per pantalla, tan com indica al void setup() a l'acció myfile = SD.open("archivo.txt"). 


## **PART B: Lectura d'etiqueta RFID**
### **6.4 Codi**
````c++
    #include <Arduino.h>  
    #include <SPI.h>  
    #include <MFRC522.h>  
    #define SS_PIN    5  // ESP32 pin GIOP5   
    #define RST_PIN   27 // ESP32 pin GIOP27   
    #define RELAY_PIN 22 // ESP32 pin GIOP22 connects to relay  

    MFRC522 rfid(SS_PIN, RST_PIN);  

    byte authorizedUID[4] = {0xFF, 0xFF, 0xFF, 0xFF};  

    void setup() {  
        Serial.begin(115200);  
        SPI.begin(); // init SPI bus  
        rfid.PCD_Init(); // init MFRC522  
        pinMode(RELAY_PIN, OUTPUT); // initialize pin as an   output.  
        digitalWrite(RELAY_PIN, LOW); // deactivate the relay    
        Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");  
    }

    void loop() {  
        if (rfid.PICC_IsNewCardPresent()) { // new tag is available  
            if (rfid.PICC_ReadCardSerial()) { // NUID has been readed  
                MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);  

                if (rfid.uid.uidByte[0] == authorizedUID[0] &&
                    rfid.uid.uidByte[1] == authorizedUID[1] &&
                    rfid.uid.uidByte[2] == authorizedUID[2] &&
                    rfid.uid.uidByte[3] == authorizedUID[3] ) {
                    Serial.println("Authorized Tag");
                    digitalWrite(RELAY_PIN, HIGH); // activate the relay for 2 seconds
                    delay(2000);
                    digitalWrite(RELAY_PIN, LOW);  // deactivate the relay
                }
                else{
                    Serial.print("Unauthorized Tag with UID:");
                    for (int i = 0; i < rfid.uid.size; i++) {
                        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
                        Serial.print(rfid.uid.uidByte[i], HEX);
                    }
                    Serial.println();
                }

                rfid.PICC_HaltA(); // halt PICC
                rfid.PCD_StopCrypto1(); // stop encryption on PCD
            }
        }
    }
````

### **6.5 Funcionament**
L'objectiu del programa és que a l'apropar la targeta al lector surti per pantalla si la targeta està autoritzada o no. Primerament es definiran els pins SS_PIN, RST_PIN que pertanyen a la trageta rfid i RELAY_PIN. És per això que apareixerà un primer missatge en el qual et demanarà que apropis la targeta al lector per poder fer la lectura. Serà en el void setup() que s'inicialitzarà el perifèric SPI rfid per començar a fer la lectura. Depenent del id de la targeta apareixerà un missatge en el que es dirà si està autoritzada: "Authorized tag" i llavors s'activarà el relay durant dos segons; o desautoritzada:"Unauthorized Tag with UID: " i també apareixerà el id de la targeta.

El video de demostració del funcionament esta penjat al github.
https://drive.google.com/file/d/1SwT0krK6JeKQlN-rZZfUBzuBF9_TlQv1/view?usp=sharing

## **Part 3: altres periferics SPI**
### **Rellotge RTC**
### **6.6 Codi**

````c++
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

```` 

### **6.8 Funcionament**
El programa compta amb un rellotge RTC per tal de mostrar per pantalla l'hora i la data. El codi inicialitza el rellotge RTC  en el void setup(). Seguidament en el bucle, void loop(), s'obte l'hora i la data amb el rellotge RTC i es carrega el missatge que s'imprimira per pantalla. Finalment ens apareixera per pantalla l'hora i la data amb el següent format: "Hora = hora:minut:segon Fecha = dia/mes/any".
