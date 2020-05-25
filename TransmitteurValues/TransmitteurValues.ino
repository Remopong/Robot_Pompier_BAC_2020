#include <NRFLite.h> // On importe la librairie qui gère les NRF24L01
#include <SHT1x.h> // On importe la librairie qui gère les

#define FLAME_SENSOR 8
#define GAZ_SENSOR A0
#define dataPin  8
#define clockPin 9
#define readWater 22
#define readFoam 23

const static uint8_t RADIO_ID_TX = 1;
const static uint8_t ID_RX_CONTROLLER = 2;
const static uint8_t PIN_RADIO_CE = 10;
const static uint8_t PIN_RADIO_CSN_TX = 11;

struct structTx
{
  uint8_t tmp;
  uint8_t flm;
  uint8_t gas;
  uint8_t water;
  uint8_t foam;
};

SHT1x sht1x(dataPin, clockPin);

structTx DataTx;
NRFLite radioTx;

void setup() {

  Serial.begin(9600);

  if (!radioTx.init(RADIO_ID_TX, PIN_RADIO_CE, PIN_RADIO_CSN_TX))
  {
    Serial.println("Fail Radio Tx");
    while (1);
  }

  pinMode(FLAME_SENSOR, INPUT);
  pinMode(GAZ_SENSOR, INPUT);

}
void loop() {

  int temp_c;
  int humidity;

  temp_c = sht1x.readTemperatureC();

  humidity = sht1x.readHumidity();

  Serial.print("Temperature: ");
  Serial.print(temp_c, DEC);
  Serial.print("C");
  Serial.print(" Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  if (DataTx.water = (digitalRead(readWater) == 1 ? 1 : 0));
  Serial.println(digitalRead(readWater));
  if (DataTx.foam = (digitalRead(readFoam) == 1 ? 1 : 0));
  Serial.println(digitalRead(readFoam));
  DataTx.tmp = (temp_c);

  if (isFlameDetected())
    flameDetected();
  else flameNotDetected();

  if (isGazDetected())
    gasDetected();
  else gasNotDetected();


if (radioTx.send(ID_RX_CONTROLLER, &DataTx, sizeof(DataTx))) {
    //Serial.println("Sent");
  } else {
    //Serial.println("send Error");
  }
  delay(750);
}
void flameDetected()
{
  DataTx.flm = 1;
  Serial.println("Flamme");
}
void flameNotDetected()
{
  DataTx.flm = 0;
  Serial.println("Pas de flamme");
}
bool isFlameDetected()
{
  if (digitalRead(FLAME_SENSOR))
    return false;
  else return true;
}

void gasDetected()
{
  DataTx.gas = 1;
  Serial.println("Gaz toxique");
}
void gasNotDetected()
{
  DataTx.gas = 0;
  Serial.println("Pas de gaz toxique");
}
bool isGazDetected()
{
  if (analogRead(GAZ_SENSOR))
    return false;
  else return true;
}
