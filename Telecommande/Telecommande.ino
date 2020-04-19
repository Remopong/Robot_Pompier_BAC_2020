#include <Esplora.h>
int joyLimit = 200;

int nbr = 0;
int binTampon[9];
int binValue[9];

int binary[15];

int binDirection[4];

int binButtons[3];

#define pinOut 7
#define pinIn 8

int i = 0;
int reset = 0;

bool unlock_i = false ;

bool UP;
bool DOWN;
bool RIGHT;
bool LEFT;
bool UP_RIGHT;
bool UP_LEFT;
bool DOWN_RIGHT;
bool DOWN_LEFT;
bool NEUTRAL;

int btn1;
int btn2;
int btn3;
int btn4;
int direct;
int angle;

void setup()
{
  Serial.begin(9600);
  pinMode(pinOut, OUTPUT);
  pinMode(pinIn, INPUT);
  Serial.print(digitalRead(pinIn));
  Serial.println("Starting up");
  digitalWrite(pinOut, 1);
}
void loop()
{

  joystick();

  //debugJoy();

  buttons();

  //debugButtons();

  slider();

  //debugSlider();

  sendIt();
}

void joystick() {
  if (NEUTRAL = (Esplora.readJoystickY() < joyLimit && Esplora.readJoystickY() > -joyLimit && Esplora.readJoystickX() < joyLimit && Esplora.readJoystickX() > -joyLimit) ? true : false) {
    direct = 0;
    fillMeUpDirect(0, 0, 0, 1);
  }
  if (UP = (Esplora.readJoystickY() < -joyLimit && Esplora.readJoystickX() > -joyLimit && Esplora.readJoystickX() < joyLimit) ? true : false) {
    direct = 1;
    fillMeUpDirect(0, 0, 1, 0);
  }
  if (DOWN = (Esplora.readJoystickY() > joyLimit && Esplora.readJoystickX() > -joyLimit && Esplora.readJoystickX() < joyLimit) ? true : false) {
    direct = 2;
    fillMeUpDirect(0, 0, 1, 1);
  }
  if (RIGHT = (Esplora.readJoystickY() > -joyLimit && Esplora.readJoystickY() < joyLimit && Esplora.readJoystickX() < -joyLimit) ? true : false) {
    direct = 3;
    fillMeUpDirect(0, 1, 0, 0);
  }
  if (LEFT = (Esplora.readJoystickY() > -joyLimit && Esplora.readJoystickY() < joyLimit && Esplora.readJoystickX() > joyLimit) ? true : false) {
    direct = 4;
    fillMeUpDirect(0, 1, 0, 1);
  }
  if (UP_RIGHT = (Esplora.readJoystickY() < -joyLimit && Esplora.readJoystickX() < -joyLimit) ? true : false) {
    direct = 5;
    fillMeUpDirect(0, 1, 1, 0);
  }
  if (UP_LEFT = (Esplora.readJoystickY() < -joyLimit && Esplora.readJoystickX() > joyLimit) ? true : false) {
    direct = 6;
    fillMeUpDirect(0, 1, 1, 1);
  }
  if (DOWN_RIGHT = (Esplora.readJoystickY() > joyLimit && Esplora.readJoystickX() < -joyLimit) ? true : false) {
    direct = 7;
    fillMeUpDirect(1, 0, 0, 0);
  }
  if (DOWN_LEFT = (Esplora.readJoystickY() > joyLimit && Esplora.readJoystickX() > joyLimit) ? true : false) {
    direct = 8;
    fillMeUpDirect(1, 0, 0, 1);
  }
}

void buttons() {

  btn1 = !Esplora.readButton(1);
  btn2 = !Esplora.readButton(2);
  btn3 = !Esplora.readButton(3);
  btn4 = !Esplora.readButton(4);

  if        (btn1) {
    fillMeUpButtons(0, 0, 1);
  } else if (btn2) {
    fillMeUpButtons(0, 1, 0);
  } else if (btn3) {
    fillMeUpButtons(0, 1, 1);
  } else if (btn4) {
    fillMeUpButtons(1, 0, 0);
  }
}

void slider() {
  angle = map(Esplora.readSlider(), 1023, 0, 0, 180);
}

void sendIt() {
  emptyAll(true, true, true, false);
  callDirect();
  callSlider();
  callButtons();

  if (digitalRead(pinIn) == 1 && unlock_i == false ) {
    digitalWrite(pinOut, binary[i]);
    Serial.print(binary[i]);
    unlock_i = true;
    i++;
    //Serial.print(reset);
    reset = 0;
  }
  if (digitalRead(pinIn) == 0) {
    unlock_i = false;
    //Serial.print(reset);
    reset++;
  }

  if ( reset > 100){
    //Serial.println("Reset");
    i = 0;
  }

  if (i == 15) {
    i = 0;
    Serial.println("");
  }
  emptyAll(false, false, false, true);
}

void callDirect() {
  for (int j = 0; j < 4; j++) {
    binary[j] = binDirection[j];
  }
}

void callSlider() {
  nbr = 0;
  if (angle < 1) {
  } else {
    while (angle > 0) {
      nbr++;
      binTampon[nbr] = angle % 2;
      angle /= 2;
      if (angle % 2 == 1) {
        angle += 0.5;
      }
    }
    binTampon[nbr] = 1;
  }
  int indexValue = 0;
  for (int k = nbr + 1; k > 1; k--) {
    binValue[indexValue] = binTampon[k];
    indexValue++;
  }
  int l = 11;
  for (int m = 1; m < indexValue + 1; m++) {
    binary[l] = binTampon[m];
    //Serial.print(binary[l]);
    l--;
  }
}

void callButtons() {
  int w = 12;
  for (int x = 0; x < 3; x++) {
    binary[w] = binButtons[x];
    w++;
  }
}

void debugSlider() {
  Serial.println(angle);
}

void debugButtons() {
  if (btn1)Serial.println("1");
  if (btn2)Serial.println("2");
  if (btn3)Serial.println("3");
  if (btn4)Serial.println("4");
}

void debugJoy() {
  switch (direct) {
    case 0: Serial.println("NEUTRAL"); break;
    case 1: Serial.println("UP"); break;
    case 2: Serial.println("DOWN"); break;
    case 3: Serial.println("RIGHT"); break;
    case 4: Serial.println("LEFT"); break;
    case 5: Serial.println("UP_RIGHT"); break;
    case 6: Serial.println("UP_LEFT"); break;
    case 7: Serial.println("DOWN_RIGHT"); break;
    case 8: Serial.println("DOWN_LEFT"); break;
  }
}

void fillMeUpDirect(int first, int second, int third, int fourth) {
  binDirection[0] = first;
  binDirection[1] = second;
  binDirection[2] = third;
  binDirection[3] = fourth;
}

void fillMeUpButtons(int first, int second, int third) {
  binButtons[0] = first;
  binButtons[1] = second;
  binButtons[2] = third;
}

void emptyAll(bool tamp, bool val, bool bin, bool butt) {
  for (int c = 0; c < 9; c++) {
    if (tamp)binTampon[c] = 0;
    if (val)binValue[c] = 0;
  }
  for (int v = 4; v < 12; v++) {
    if (bin) binary[v] = 0;
  }
  if(butt) fillMeUpButtons(0, 0, 0);
}
