// C++ code
//


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
const SensorMap PHOTORESISITOR(100, 1769, 0, 100);



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

    Serial.println(leitura);
 	return leitura;
  }
  
};


void debug(const Dado& dado) {
  switch(dado.unidade) {
    case Unidade::UMIDADE:
    	Serial.print("Umidade do solo:");
    default:
    	Serial.print("Unidade Desconhecida");
  }
  Serial.println(dado.valor);
}

Sensor sensor(A1,
