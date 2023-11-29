#include <DHT.h>

enum Unidade{
  CELCIUS,
  UMIDADE,
  LUZ
};

class Dado{
public:
  Dado(float valor, Unidade unidade){
    this->valor = valor;
    this->unidade = unidade;
  }
    
  float valor;
  Unidade unidade;
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
        return map(valor, inMin, inMax, outMin, outMax);
    }
};

const SensorMap SOIL_MOISTURE_SENSOR(0, 4096, 0, 100);
const SensorMap LDR_SENSOR(0, 4096, 0, 100);


class Isensor{
public:
  virtual  float leitura() = 0;
  virtual Dado ler() = 0;
};

class Sensor : public Isensor{
private:    
  const int pin;
  const Unidade unidade;
  const SensorMap& mapper;
    
public:
  Sensor(int pin, Unidade unidade, const SensorMap& mapper) : pin(pin), unidade(unidade), mapper(mapper) { pinMode(this->pin, INPUT); }
    
  virtual float leitura(){
    float dado = analogRead(this->pin);
    float leitura = this->mapper.mapear(dado);
    return leitura;
  }
    
  virtual Dado ler() override {
    float valor = this->leitura();
    return Dado(valor, this->unidade);
  }
};

class SensorDHT : public Isensor{
private:
  DHT dht;
  float temperatura;
  float umidade;

public:
  SensorDHT(int pin) : dht(pin, DHT22) {
    dht.begin();
    //leituraDHT;
    temperatura;
    umidade;
  }

  float leitura(){
    return 0;
  }

  void verificacaoDHT() {
    umidade = dht.readHumidity();
    temperatura = dht.readTemperature();

    if (isnan(temperatura) || isnan(umidade)) {
      Serial.println("ERRO AO LER O SENSOR DHT");
    }
  }

  virtual Dado ler() {
    float temperatura=lerTemperatura();
    return Dado(temperatura,CELCIUS);
  }

  float lerTemperatura() {
    float temperatura = dht.readTemperature();
    return temperatura;
  }

  float lerUmidade() {
    umidade = dht.readHumidity();
    return umidade;
  }
 
};
