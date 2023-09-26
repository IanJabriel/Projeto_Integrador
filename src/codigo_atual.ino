#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "REDE";
const char* password = "SENHA DA REDE";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient wifiClient;
PubSubClient client(wifiClient); 
char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setupMQTT() {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}


void setup_wifi() {
  delay(10);
    Serial.println();
  Serial.print("Conectando na rede ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("Endereço IP : ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Menssagem recebida [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT ...");
    String clientId = "GRUPO_007";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      client.subscribe("grupoXYZ/publisher");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}


enum Unidade
{
  UMIDADE,
  CELSIUS

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


const SensorMap SOIL_MOISTURE_SENSOR(0, 872, 0, 100);


class Sensor
{
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

Sensor(13,Unidade UMIDADE,const SensorMap& mapper);
Sensor sensorTeste(13,UMIDADE,SOIL_MOISTURE_SENSOR);

void setup() {
  Serial.begin(5760);
  setup_wifi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
}

int umi_solo = 0;
void loop()
{
  if (!client.connected())
    reconnect();
  client.loop();
  long now = millis();
  if (now - lastMsg > 60000) {
    
    lastMsg = now;
    delay(10);
  }

  umi_solo = sensorTeste.leituraGenerica();
  delay(300);
  
  if (umi_solo < 4095 && umi_solo > 2200){
    Serial.print("ÚMIDO");
    snprintf(msg,MSG_BUFFER_SIZE," Solo úmido");
    client.publish("grupoXYZ/solo",msg);
  }
  else if(umi_solo <= 2200 && umi_solo > 500){
    Serial.print("ENCHARCADO");
    snprintf(msg,MSG_BUFFER_SIZE,"Solo encharcado");
    client.publish("grupoXYZ/solo",msg);
  }
  else{
    Serial.print("SECO ");
    snprintf(msg,MSG_BUFFER_SIZE,"Solo seco");
    client.publish("grupoXYZ/solo",msg); 
  }

  Serial.print(" Umiddade: ");
  Serial.print(umi_solo);
  Serial.println("");
}