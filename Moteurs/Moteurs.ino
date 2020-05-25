#include <Servo.h> // On importe la librairie qui gère les
#include <NRFLite.h> // On importe la librairie qui gère les NRF24L01

const static uint8_t RADIO_ID_RX = 0;
const static uint8_t ID_RX_CONTROLLER = 2;
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN_RX = 7;

struct structRx
{
  uint8_t btn1;
  uint8_t btn2;
  uint8_t btn3;
  uint8_t btn4;
  uint8_t direct;
  uint8_t angle;
};

Servo elevatorMotor;


NRFLite radioRx;
structRx DataRx;

#define leftMotors  5       // e1 BLEU
#define LeftMotorsState  4  // m1 VERT
#define RightMotors  3      // e2 ROUGE
#define RightMotorsState  2 // m2 VIOLET

#define MAX 255
#define SLOWER 130
#define STOP 0

bool forwardState = true;
bool reverseState = false;

int WaterPin = 8;
int FoamPin = 10;

void setup()
{
  Serial.begin(9600);

  pinMode(WaterPin, OUTPUT);
  pinMode(FoamPin, OUTPUT);
  digitalWrite(WaterPin, LOW);
  digitalWrite(FoamPin, LOW);

  elevatorMotor.attach(6);

  if (!radioRx.init(RADIO_ID_RX, PIN_RADIO_CE, PIN_RADIO_CSN_RX))
  {
    Serial.println("Fail Radio Rx");
    while(1);
  }

  pinMode(LeftMotorsState, OUTPUT);
  pinMode(RightMotorsState, OUTPUT);

}
void loop()
{
  while (radioRx.hasData())
  {
    radioRx.readData(&DataRx);
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
    default: wait();
      break;
  }

  if (DataRx.btn1)digitalWrite(WaterPin, HIGH);
  if (DataRx.btn2)digitalWrite(WaterPin, LOW);
  if (DataRx.btn3)digitalWrite(FoamPin, HIGH);
  if (DataRx.btn4)digitalWrite(FoamPin, LOW);

  elevatorMotor.write(DataRx.angle);

}
/********************************/
void forward() {
  digitalWrite(LeftMotorsState, forwardState);
  digitalWrite(RightMotorsState, forwardState);
  analogWrite(leftMotors, MAX);
  analogWrite(RightMotors, MAX);
}
void forwardRight() {
  digitalWrite(LeftMotorsState, forwardState);
  digitalWrite(RightMotorsState, forwardState);
  analogWrite(leftMotors, MAX);
  analogWrite(RightMotors, SLOWER);
}
void forwardLeft() {
  digitalWrite(LeftMotorsState, forwardState);
  digitalWrite(RightMotorsState, forwardState);
  analogWrite(leftMotors, SLOWER);
  analogWrite(RightMotors, MAX);
}
void Right() {
  digitalWrite(LeftMotorsState, forwardState);
  digitalWrite(RightMotorsState, reverseState);
  analogWrite(leftMotors, MAX);
  analogWrite(RightMotors, MAX);
}
void Left() {
  digitalWrite(LeftMotorsState, reverseState);
  digitalWrite(RightMotorsState, forwardState);
  analogWrite(leftMotors, MAX);
  analogWrite(RightMotors, MAX);
}
void reverse() {
  digitalWrite(LeftMotorsState, reverseState);
  digitalWrite(RightMotorsState, reverseState);
  analogWrite(leftMotors, MAX);
  analogWrite(RightMotors, MAX);
}
void reverseRight() {
  digitalWrite(LeftMotorsState, reverseState);
  digitalWrite(RightMotorsState, reverseState);
  analogWrite(leftMotors, MAX);
  analogWrite(RightMotors, SLOWER);
}
void reverseLeft() {
  digitalWrite(LeftMotorsState, reverseState);
  digitalWrite(RightMotorsState, reverseState);
  analogWrite(leftMotors, SLOWER);
  analogWrite(RightMotors, MAX);
}
void wait() {
  analogWrite(leftMotors, 0);
  analogWrite(RightMotors, 0);
}
