#include<DHT.h>

enum Unidade{
  UMIDADE,
  CELSIUS,
  LUZ

} ;


class Dado {
public:
  float valor;
  Unidade unidade;
  Dado (float valor, Unidade unidade) {
    this->valor = valor;
    this->unidade = unidade;
  }
};


class SensorMap {
private:
  int inMax;
  int inMin;
  float outMin;
  float outMax;


 public:
  SensorMap(
    int inMax,
    int inMin,
    float outMin,
    float outMax
  ) {
    this->inMax = inMax;
    this->inMin = inMin;
    this->outMin = outMin;
    this->outMax = outMax;
  }
 
  float mapear(int valor) const {
    return map(valor, inMin ,inMax, outMin, outMax);
  }


};




const SensorMap SOIL_MOISTURE_SENSOR(0,4096 , 0, 100);
const SensorMap LDR_SENSOR(0,4096,0,100);




class Sensor{
private:


  const int pin;
  const Unidade unidade;
  const SensorMap& mapper;
 
  public:
 
  Sensor(int pin, Unidade unidade, const SensorMap& mapper) : pin(pin), unidade(unidade), mapper(mapper) {pinMode(this->pin,INPUT);}
 
 
  float leituraGenerica(){
    float dado=analogRead(this->pin);
    float leitura=this->mapper.mapear(dado);
    return leitura;
  }
  
    Dado ler() {
    float valor = this->leituraGenerica();
    return Dado(valor, this->unidade);
  }
};


class SensorDHT {
private:
  DHT dht;
  float temperatura;
  float umidade;

public:
  SensorDHT(int pin) : dht(pin, DHT22) {
    dht.begin();
    temperatura;
    umidade;
  }

  float leituraDHT() {
    umidade = dht.readHumidity();
    temperatura = dht.readTemperature();
    
    if (isnan(temperatura) || isnan(umidade)) {
      Serial.println("ERRO AO LER O SENSOR DHT");
    }
  }

  float lerTemperatura() {
    temperatura = dht.readTemperature();
    return temperatura;
  }

  float lerUmidade() {
    umidade = dht.readHumidity();
    return umidade;
  }

  Dado lerDHT() {
    float valor = this->leituraDHT();
    return Dado(valor, UMIDADE); 
  }
};

SensorDHT sensorDHT(26);

//Sensor(13,Unidade UMIDADE,const SensorMap& mapper);
Sensor sensorTeste(34,UMIDADE,SOIL_MOISTURE_SENSOR);
Sensor sensorLDR(35,LUZ,LDR_SENSOR);

int led_ldr = 6;

void setup() {
  pinMode(led_ldr,OUPUT);
  Serial.begin(57600);
  //setup_wifi();
  //client.setServer(mqttServer, mqttPort);
  //client.setCallback(callback);
}


int umi_solo = 0;
int ldr = 0;


void loop()
{
 /*if (!client.connected())
    reconnect();
  client.loop();
  long now = millis();
  if (now - lastMsg > 60000) {
   
    lastMsg = now;
    delay(10);
  }*/


  umi_solo = sensorTeste.leituraGenerica();
  delay(300);
 
  if (umi_solo <= 100 && umi_solo > 70){
    Serial.print("encharcado");
    //snprintf(msg,MSG_BUFFER_SIZE," Solo úmido");
    //client.publish("grupoXYZ/solo",msg);
  }
  else if(umi_solo <= 70 && umi_solo > 35){
    Serial.print("ÚMIDO");
    //snprintf(msg,MSG_BUFFER_SIZE,"Solo encharcado");
    //client.publish("grupoXYZ/solo",msg);
  }
  else{
    Serial.print("SECO ");
    //snprintf(msg,MSG_BUFFER_SIZE,"Solo seco");
    //client.publish("grupoXYZ/solo",msg);
  }

  Serial.print(" Umiddade: ");
  Serial.print(umi_solo);
  Serial.println("");

  Serial.println(analogRead(34));

  
  ldr = sensorLDR.leituraGenerica();
  delay(300);
  
  if(ldr <= 100 && ldr > 60){
  	digitalWrite(led_ldr,HIGH);
    delay(200);
  }
  else{
  	digitalWrite(led_ldr,LOW);
    delay(200);
  }
  Serial.println(analogRead(35));


  sensorDHT.leituraDHT();
  float temp = sensorDHT.lerTemperatura();
  float umid = sensorDHT.lerTemperatura();

  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.println("°C");
  Serial.print("Umidade do ar: ");
  Serial.print(umid);
  Serial.println("%");
}