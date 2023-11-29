#include "classes.h"
#include"conexao.h"

SensorDHT sensorDHT(26);
Sensor sensorTeste(34, UMIDADE, SOIL_MOISTURE_SENSOR);
Sensor sensorLDR(35, LUZ, LDR_SENSOR);

int umi_solo = 0;
int ldr = 0;
float value = 0;

void setup() {
    Serial.begin(57600);
    pinMode(bomba,OUTPUT);
    digitalWrite(bomba,LOW);
    setup_wifi();
    setupMQTT();
    delay(500);
}

void loop() {
    if (!client.connected())
      reconnect();
    client.loop();
    long now = millis();
    if (now - lastMsg > 60000) {
      
      lastMsg = now;
      delay(10);
    }

        
    umi_solo = sensorTeste.leitura();
    delay(500);

    if (umi_solo <= 100 && umi_solo > 70) {
        Serial.print("encharcado");
        snprintf(msg,MSG_BUFFER_SIZE,"Solo encharcado");
        client.publish("grupoXYZ/solo",msg);
    }
    else if (umi_solo <= 70 && umi_solo > 35) {
        Serial.print("ÚMIDO");
        snprintf(msg,MSG_BUFFER_SIZE," Solo úmido");
        client.publish("grupoXYZ/solo",msg);
    }
    else {
        Serial.print("SECO ");
        snprintf(msg,MSG_BUFFER_SIZE,"Solo seco");
        client.publish("grupoXYZ/solo",msg); 
    }

    Serial.print(" Umiddade: ");
    Serial.print(umi_solo);
    Serial.println("");


    ldr = sensorLDR.leitura();
    delay(1500);
    int aux_ldr = ldr;

    if (aux_ldr <= 100 && aux_ldr > 60) {
        Serial.println("Muita luz");
        snprintf(msg,MSG_BUFFER_SIZE,"Muita luz");
        client.publish("grupoXYZ/luz",msg); 
    }
    else {
        Serial.println("Pouca luz");
        snprintf(msg,MSG_BUFFER_SIZE,"Pouca luz");
        client.publish("grupoXYZ/luz",msg);
    }
    Serial.println("");


    sensorDHT.verificacaoDHT();
    float temp = sensorDHT.lerTemperatura();
    float umid = sensorDHT.lerUmidade();

    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.println("°C");
    Serial.print("Umidade do ar: ");
    Serial.print(umid);
    Serial.println("%");
    Serial.println("");

    snprintf(msg,MSG_BUFFER_SIZE,"%.1f",temp);
    client.publish("grupoXYZ/temper",msg);

    snprintf(msg,MSG_BUFFER_SIZE,"%.1f",umid);
    client.publish("grupoXYZ/umid",msg);

      
    delay(1500);

    if (!client.connected()) {
      reconnect();
    }
    client.loop();
}
