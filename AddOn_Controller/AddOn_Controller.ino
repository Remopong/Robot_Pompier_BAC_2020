#include <NRFLite.h>

//int test[15] = {0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1}; // NEUTRAL + 90 + BTN1

int enter[15];

int breakOut = 0;

int value;

#define pinIn 6
#define pinOut 5

int i = 0;

bool block = false;
const static uint8_t RADIO_ID_TX = 3;// Our radio's id.
const static uint8_t RADIO_ID_RX = 2;
const static uint8_t ID_RX_VEHICLE = 0; // Id of the radio we will transmit to.
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN_TX = 8;
const static uint8_t PIN_RADIO_CSN_RX = 7;

struct structRx // Any packet up to 32 bytes can be sent.
{
  uint8_t tmp;
  uint8_t flm;
  uint8_t gas;
  uint8_t water;
  uint8_t foam;
  uint32_t FailedTxCount;
};

struct structTx // Any packet up to 32 bytes can be sent.
{
  uint8_t btn1;
  uint8_t btn2;
  uint8_t btn3;
  uint8_t btn4;
  uint8_t direct;
  uint8_t angle;
  uint32_t FailedTxCount;
};

NRFLite radioTx;
NRFLite radioRx;
structTx DataTx;
structRx DataRx;

void setup() {

  Serial.begin(9600);

  pinMode(pinIn, INPUT);
  pinMode(pinOut, OUTPUT);
  digitalWrite(pinOut, 0);
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
    //while (1);
  }
  delay(1000);
}

void loop() {

  digitalWrite(pinOut, 1);
  delay(4);
  enter[i] = digitalRead(pinIn);
  //Serial.print(enter[i]);
  digitalWrite(pinOut, 0);
  delay(4);
  i++;

  if (i == 15) {
    i = 0;
    //Serial.println("");
  }

  if          (enter[0] == 0 && enter[1] == 0 && enter[2] == 0 && enter[3] == 1) {
    Serial.println("NEUTRAL");
    DataTx.direct = 0;
  } else if   (enter[0] == 0 && enter[1] == 0 && enter[2] == 1 && enter[3] == 0) {
    Serial.println("UP");
    DataTx.direct = 1;
  } else if   (enter[0] == 0 && enter[1] == 0 && enter[2] == 1 && enter[3] == 1) {
    Serial.println("DOWN");
    DataTx.direct = 6;
  } else if   (enter[0] == 0 && enter[1] == 1 && enter[2] == 0 && enter[3] == 0) {
    Serial.println("RIGHT");
    DataTx.direct = 4;
  } else if   (enter[0] == 0 && enter[1] == 1 && enter[2] == 0 && enter[3] == 1) {
    Serial.println("LEFT");
    DataTx.direct = 5;
  } else if   (enter[0] == 0 && enter[1] == 1 && enter[2] == 1 && enter[3] == 0) {
    Serial.println("UP_RIGHT");
    DataTx.direct = 2;
  } else if   (enter[0] == 0 && enter[1] == 1 && enter[2] == 1 && enter[3] == 1) {
    Serial.println("UP_LEFT");
    DataTx.direct = 3;
  } else if   (enter[0] == 1 && enter[1] == 0 && enter[2] == 0 && enter[3] == 0) {
    Serial.println("DOWN_RIGHT");
    DataTx.direct = 7;
  } else if   (enter[0] == 1 && enter[1] == 0 && enter[2] == 0 && enter[3] == 1) {
    Serial.println("DOWN_LEFT");
    DataTx.direct = 8;
  } else {
    Serial.println("ERROR");
  }

  int multi = 1;
  value = 0;
  for (int m = 11; m > 3; m-- ) {
    value += enter[m] * multi;
    multi += multi;
  }
  DataTx.angle = value;

  //Serial.println(value);

  if (enter[12] == 0 && enter[13] == 0 && enter[14] == 1) {
    DataTx.btn1 = 1;
    //Serial.println("BTN1");
  } else {
    DataTx.btn1 = 0;
  }
  if (enter[12] == 0 && enter[13] == 1 && enter[14] == 0) {
    DataTx.btn2 = 1;
    //Serial.println("BTN2");
  } else {
    DataTx.btn2 = 0;
  }
  if (enter[12] == 0 && enter[13] == 1 && enter[14] == 1) {
    DataTx.btn3 = 1;
    //Serial.println("BTN3");
  } else {
    DataTx.btn3 = 0;
  }
  if (enter[12] == 1 && enter[13] == 0 && enter[14] == 0) {
    DataTx.btn4 = 1;
    //Serial.println("BTN4");
  } else {
    DataTx.btn4 = 0;
  }
  if (radioTx.send(ID_RX_VEHICLE, &DataTx, sizeof(DataTx))) // Note how '&' must be placed in front of the variable name.
  {
    Serial.println("...Success");
  }
  else
  {
    Serial.println("...Failed");
    DataTx.FailedTxCount++;
  }
}
