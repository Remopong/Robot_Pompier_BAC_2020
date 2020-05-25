#include <NRFLite.h> // On importe la librairie qui gère les NRF24L01
#include <ShiftLCD.h>
ShiftLCD lcd(2, 4, 3);
int prct = 0;
int counter = 0;
bool show = false;
int FailedTxCount = 0;

String txt;
String flmState, gasState, waterState, foamState;

int period = 4000;
unsigned long time_now = 0;

int enter[15];

int breakOut = 0;

int value;

int tampon;

char data[20];

#define pinIn 6
#define pinOut 5

int i = 0;

bool block = false;
const static uint8_t RADIO_ID_TX = 3;
const static uint8_t RADIO_ID_RX = 2;
const static uint8_t ID_RX_VEHICLE = 0;
const static uint8_t PIN_RADIO_CE = 9;
const static uint8_t PIN_RADIO_CSN_TX = 8;
const static uint8_t PIN_RADIO_CSN_RX = 7;

struct structRx
{
  uint8_t tmp;
  uint8_t flm;
  uint8_t gas;
  uint8_t water;
  uint8_t foam;
};

struct structTx
{
  uint8_t btn1;
  uint8_t btn2;
  uint8_t btn3;
  uint8_t btn4;
  uint8_t direct;
  uint8_t angle;
};

NRFLite radioTx;
NRFLite radioRx;
structTx DataTx;
structRx DataRx;

void setup() {

  Serial.begin(9600);

  lcd.begin(20, 4);
  pinMode(pinIn, INPUT);
  pinMode(pinOut, OUTPUT);
  digitalWrite(pinOut, 0);
  if (!radioTx.init(RADIO_ID_TX, PIN_RADIO_CE, PIN_RADIO_CSN_TX))
  {
    Serial.println("Fail Radio Tx");
    block = true;
  }
  if (!radioRx.init(RADIO_ID_RX, PIN_RADIO_CE, PIN_RADIO_CSN_RX))
  {
    Serial.println("Fail Radio Rx");
    block = true;
  }

  if ( block == true) {
    while (block == true) {
      Serial.println("-");
      delay(1000);
    }
  }
  delay(1000);
}

void loop() {

  digitalWrite(pinOut, 1);
  delay(4);
  enter[i] = digitalRead(pinIn);
  digitalWrite(pinOut, 0);
  delay(4);
  i++;

  if (i == 15) {
    i = 0;
  }

  if          (enter[0] == 0 && enter[1] == 0 && enter[2] == 0 && enter[3] == 1) {
    DataTx.direct = 0;
  } else if   (enter[0] == 0 && enter[1] == 0 && enter[2] == 1 && enter[3] == 0) {
    DataTx.direct = 1;
  } else if   (enter[0] == 0 && enter[1] == 0 && enter[2] == 1 && enter[3] == 1) {
    DataTx.direct = 6;
  } else if   (enter[0] == 0 && enter[1] == 1 && enter[2] == 0 && enter[3] == 0) {
    DataTx.direct = 4;
  } else if   (enter[0] == 0 && enter[1] == 1 && enter[2] == 0 && enter[3] == 1) {
    DataTx.direct = 5;
  } else if   (enter[0] == 0 && enter[1] == 1 && enter[2] == 1 && enter[3] == 0) {
    DataTx.direct = 2;
  } else if   (enter[0] == 0 && enter[1] == 1 && enter[2] == 1 && enter[3] == 1) {
    DataTx.direct = 3;
  } else if   (enter[0] == 1 && enter[1] == 0 && enter[2] == 0 && enter[3] == 0) {
    DataTx.direct = 7;
  } else if   (enter[0] == 1 && enter[1] == 0 && enter[2] == 0 && enter[3] == 1) {
    DataTx.direct = 8;
  } else {
  }

  int multi = 1;
  value = 0;
  for (int m = 11; m > 3; m-- ) {
    value += enter[m] * multi;
    multi += multi;
  }
  DataTx.angle = value;

  if (enter[12] == 0 && enter[13] == 0 && enter[14] == 1) {
    DataTx.btn1 = 1;
  } else {
    DataTx.btn1 = 0;
  }
  if (enter[12] == 0 && enter[13] == 1 && enter[14] == 0) {
    DataTx.btn2 = 1;
  } else {
    DataTx.btn2 = 0;
  }
  if (enter[12] == 0 && enter[13] == 1 && enter[14] == 1) {
    DataTx.btn3 = 1;
  } else {
    DataTx.btn3 = 0;
  }
  if (enter[12] == 1 && enter[13] == 0 && enter[14] == 0) {
    DataTx.btn4 = 1;
  } else {
    DataTx.btn4 = 0;
  }

  if (millis() >= time_now + period) {
    time_now += period;
    lcd.clear();
    lcd.setCursor(9, 1);
    lcd.print("No");
    lcd.setCursor(5, 2);
    lcd.print("Connection");
  }

  radioRx.startRx();

  tampon = radioRx.hasData();

  if ( tampon > 0 ) {
    radioRx.readData(&DataRx);
    if (DataRx.tmp > 0) {

      time_now += period;

      if (DataRx.flm == 0)
        flmState = "NO";
      else
        flmState = "YES";

      if (DataRx.gas == 0)
        gasState = "NO";
      else
        gasState = "YES";

      if (DataRx.water == 0)
        waterState = "OFF";
      else
        waterState = "ON";

      if (DataRx.foam == 0)
        foamState = "OFF";
      else
        foamState = "ON";
      lcd.clear();
      Serial.println("Time");
      for (int writer = 0; writer < 4; writer ++) {

        switch (writer) {
          case 0: txt = "Temp : " + String(DataRx.tmp);
            if (show == true) {
              prct *= 10;
              lcd.setCursor(16, 0);
              lcd.print(prct);
              lcd.setCursor(19, 0);
              lcd.print("%");
              counter = 0;
              prct = 0;
              show = false;
            }
            break;
          case 1: lcd.setCursor(0, 1); txt = "Fire : " + flmState; break;
          case 2: txt = "Gas  : " + gasState; break;
          case 3: txt = "Water: " + waterState + " Foam: " + foamState; break;
        }
        lcd.setCursor(0, writer);
        lcd.print(txt);
      }
    }
  } else {
    radioTx.startSend(ID_RX_VEHICLE, &DataTx, sizeof(DataTx));
    if (radioTx.send(ID_RX_VEHICLE, &DataTx, sizeof(DataTx)))
    {
      if (show == false) {
        prct ++;
        counter ++;
      }
    }
    else
    {
      FailedTxCount++;
      if (show == false) {
        if (prct > 0)
          prct --;
        counter ++;
      }
    }
    if (counter == 10) {
      show = true;
    }
  }
}
