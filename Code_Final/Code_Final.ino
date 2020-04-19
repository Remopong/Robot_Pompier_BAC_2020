#include <SHT1x.h>
#include <SPI.h>
#include <NRFLite.h>
#include <Servo.h>

bool block = false;
const static uint8_t RADIO_ID_TX = 1;// Our radio's id.
const static uint8_t RADIO_ID_RX = 0;
const static uint8_t ID_RX_CONTROLLER = 2; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN_TX = 10;
const static uint8_t PIN_RADIO_CSN_RX = 7;

struct structTx // Any packet up to 32 bytes can be sent.
{
  uint8_t tmp;
  uint8_t flm;
  uint8_t gas;
  uint8_t water;
  uint8_t foam;
  uint32_t FailedTxCount;
};

struct structRx // Any packet up to 32 bytes can be sent.
{
  uint8_t btn1;
  uint8_t btn2;
  uint8_t btn3;
  uint8_t btn4;
  uint8_t direct;
  uint8_t angle;
  uint32_t FailedTxCount;
};

#define FLAME_SENSOR 8 //connect SENSOR to digital pin 8
#define LED_Flame 22 //connect Grove - LED to pin 22

#define GAZ_SENSOR A0 //connect SENSOR to analog 0
#define LED_Gaz 23 //connect Grove - LED to pin 23

// Specify data and clock connections and instantiate SHT1x object
#define dataPin  10
#define clockPin 11

SHT1x sht1x(dataPin, clockPin);
Servo elevatorMotor;

NRFLite radioTx;
NRFLite radioRx;
structTx DataTx;
structRx DataRx;

int leftMotors = 5;
int LeftMotorsState = 4;
int RightMotors = 3;
int RightMotorsState = 2;

int MAX = 255;
int SLOWER = MAX / 4;
int STOP = 0;

bool forwardState = true;
bool reverseState = false;

int localFlame;
int localGas;

int WaterPin = 24;
int FoamPin = 25;

void setup()
{
  Serial.begin(9600);

  pinMode(WaterPin, OUTPUT);
  pinMode(FoamPin, OUTPUT);
  digitalWrite(WaterPin, LOW);
  digitalWrite(FoamPin, LOW);

  elevatorMotor.attach(6);

  if (!radioTx.init(RADIO_ID_TX, PIN_RADIO_CE, PIN_RADIO_CSN_TX))
  {
    Serial.println("Fail Radio Tx");
    bool block = true;
  }
  if (!radioRx.init(RADIO_ID_RX, PIN_RADIO_CE, PIN_RADIO_CSN_RX))
  {
    Serial.println("Fail Radio Rx");
    bool block = true;
  }
  if ( block == true) {
    while (1);
  }
  pinMode(FLAME_SENSOR, INPUT);
  pinMode(LED_Flame, OUTPUT);
  digitalWrite(LED_Flame, LOW);

  pinMode(GAZ_SENSOR, INPUT);
  pinMode(LED_Gaz, OUTPUT);
  digitalWrite(LED_Gaz, LOW);

  pinMode(LeftMotorsState, OUTPUT);
  pinMode(RightMotorsState, OUTPUT);
  pinMode(leftMotors, OUTPUT);
  pinMode(RightMotors, OUTPUT);

  Serial.println("Starting up");
}
void loop()
{
  while (radioRx.hasData())
  {
    radioRx.readData(&DataRx); // Note how '&' must be placed in front of the variable name.
  }
  switch (DataRx.direct) {
    case 1: forward();
      break;
    case 2: forwardRight();
      break;
    case 3: forwardLeft();
      break;
    case 4: Right();
      break;
    case 5: Left();
      break;
    case 6: reverse();
      break;
    case 7: reverseRight();
      break;
    case 8: reverseLeft();
      break;
    case 0: wait();
      break;
  }

  if (DataRx.btn1)digitalWrite(WaterPin, HIGH);
  if (DataRx.btn2)digitalWrite(WaterPin, LOW);
  if (DataRx.btn3)digitalWrite(FoamPin, HIGH);
  if (DataRx.btn4)digitalWrite(FoamPin, LOW);

  elevatorMotor.write(DataRx.angle);

  if (isFlameDetected())
    turnOnLED_Flame();
  else turnOffLED_Flame();

  if (isGazDetected())
    turnOnLED_Gaz();
  else turnOffLED_Gaz();

  int temp_c;
  int humidity;

  temp_c = sht1x.readTemperatureC();

  humidity = sht1x.readHumidity();

  //Serial.print("Temperature: ");
  //Serial.print(temp_c, DEC);
  //Serial.print("C / ");
  //Serial.print(temp_f, DEC);
  //Serial.print(" Humidity: ");
  //Serial.print(humidity);
  //Serial.println("%");
  DataTx.tmp = (temp_c, DEC);
  DataTx.flm = localFlame;
  DataTx.gas = localGas;
  
  Serial.println(DataRx.direct);
  //Serial.println(DataRx.angle);

}
/********************************/
void turnOnLED_Flame()
{
  digitalWrite(LED_Flame, HIGH);
  localFlame = 1;
  //Serial.println("flamme");
}
void turnOffLED_Flame()
{
  digitalWrite(LED_Flame, LOW);
  localFlame = 0;
  //Serial.println("Pas de flamme");
}
boolean isFlameDetected()
{
  if (digitalRead(FLAME_SENSOR))
    return false;
  else return true;
}

/********************************/
void turnOnLED_Gaz()
{
  digitalWrite(LED_Gaz, HIGH);
  localGas = 1;
  //Serial.println("Gaz toxique present");
}
void turnOffLED_Gaz()
{
  digitalWrite(LED_Gaz, LOW);
  localGas = 0;
  //Serial.println("Pas de gaz toxique");
}
boolean isGazDetected()
{
  if (analogRead(GAZ_SENSOR))
    return false;
  else return true;
}
void forward() {
  digitalWrite(LeftMotorsState, forwardState);
  digitalWrite(RightMotorsState, !forwardState);
  analogWrite(leftMotors, MAX);
  analogWrite(RightMotors, MAX);
}
void forwardRight() {
  digitalWrite(LeftMotorsState, forwardState);
  digitalWrite(RightMotorsState, !forwardState);
  analogWrite(leftMotors, MAX);
  analogWrite(RightMotors, SLOWER);
}
void forwardLeft() {
  digitalWrite(LeftMotorsState, forwardState);
  digitalWrite(RightMotorsState, !forwardState);
  analogWrite(leftMotors, SLOWER);
  analogWrite(RightMotors, MAX);
}
void Right() {
  digitalWrite(LeftMotorsState, forwardState);
  digitalWrite(RightMotorsState, !reverseState);
  analogWrite(leftMotors, MAX);
  analogWrite(RightMotors, MAX);
}
void Left() {
  digitalWrite(LeftMotorsState, reverseState);
  digitalWrite(RightMotorsState, !forwardState);
  analogWrite(leftMotors, MAX);
  analogWrite(RightMotors, MAX);
}
void reverse() {
  digitalWrite(LeftMotorsState, reverseState);
  digitalWrite(RightMotorsState, !reverseState);
  analogWrite(leftMotors, MAX);
  analogWrite(RightMotors, MAX);
}
void reverseRight() {
  digitalWrite(LeftMotorsState, reverseState);
  digitalWrite(RightMotorsState, !reverseState);
  analogWrite(leftMotors, MAX);
  analogWrite(RightMotors, SLOWER);
}
void reverseLeft() {
  digitalWrite(LeftMotorsState, reverseState);
  digitalWrite(RightMotorsState, !reverseState);
  analogWrite(leftMotors, SLOWER);
  analogWrite(RightMotors, MAX);
}
void wait() {
  analogWrite(leftMotors, STOP);
  analogWrite(RightMotors, STOP);
}
