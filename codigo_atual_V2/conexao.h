#include <WiFi.h>
#include <PubSubClient.h>

const int bomba = 14;

const char* ssid = "SSID";
const char* password = "PASSWORD";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient wifiClient;
PubSubClient client(wifiClient); 
char* mqttServer = "broker.hivemq.com";
int mqttPort = 1883;
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

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
 Serial.println("Ligando conexão");
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

    if((char)payload[0] == '1'){
      digitalWrite(14,HIGH);
      //ligarBomba();
      Serial.println("Ligando bomba");
      //((char)payload[0] = '0';
    }
    else if((char)payload[0] == '0'){
      digitalWrite(14,LOW); 
      Serial.println("Desligando bomba");
      }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT ...");
    String clientId = "GRUPO_007";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      client.subscribe("grupoXYZ/irrigacao");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setupMQTT() {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}
