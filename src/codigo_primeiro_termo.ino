#include <WiFi.h>
#include <PubSubClient.h>

#define sensorChuva 36
#define sensorSolo 34

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
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT ...");
    // Create a random client ID
    String clientId = "GRUPO_007";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      client.subscribe("grupoXYZ/publisher");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

int humidity = 0;
int chuva = 0;
int umi_solo = 0;

void loop() {
  if (!client.connected())
    reconnect();
  client.loop();
  long now = millis();
  if (now - lastMsg > 60000) {
    
  lastMsg = now;
  delay(10); // Wait for 10 millisecond(s)0,
  }
 
  chuva = analogRead(sensorChuva);
  delay (300);

  if (chuva < 4095 && chuva > 2200){
    Serial.print("CHUVISCANDO ");
    snprintf(msg,MSG_BUFFER_SIZE,"Chuviscando");
    client.publish("grupoXYZ/chuva",msg);   
  }
  else if (chuva <= 2200 and chuva > 500){
    Serial.print("CHOVENDO ");
    snprintf(msg,MSG_BUFFER_SIZE,"Está chovendo");
    client.publish("grupoXYZ/chuva",msg);
  }
  else{
    Serial.print("NÃO ESTÁ CHOVENDO ");
    snprintf(msg,MSG_BUFFER_SIZE,"Não está chovendo");
    client.publish("grupoXYZ/chuva",msg);
  }

  Serial.print("Analógico: ");
  Serial.print(analogRead(sensorChuva));
  Serial.println("");

  umi_solo = analogRead(sensorSolo);
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

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 }
  
void setup() {
  pinMode(sensorChuva , INPUT);
  pinMode(sensorSolo, INPUT);
  Serial.begin(57600);
  setup_wifi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  delay(5000);
}
