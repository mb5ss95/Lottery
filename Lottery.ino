#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define BTN_PIN         2
#define NUMPIXELS      14
#define SHAKING_NUM   150
#define DELAYVAL      500


const int NEO_PIN[] = {6, 7, 8, 9, 10, 11};
const char *MODELS[46] = {
  "abcdefhijklmz",      // 0
  "abcdefjkz",          // 1
  "abcdefijlmnz",       // 2
  "abcdefijklnz",       // 3
  "abcdefhjknz",        // 4
  "abcdefhiklnz",       // 5
  "abcdefhiklmnz",      // 6
  "abcdefhijkz",        // 7
  "abcdefhijklmnz",     // 8
  "abcdefhijknz",       // 9
  "cdhijklmz",          // 10
  "cdjkz",              // 11
  "cdijlmnz",           // 12
  "cdijklnz",           // 13
  "cdhjknz",            // 14
  "cdhiklnz",           // 15
  "cdhiklmnz",          // 16
  "cdhijkz",            // 17
  "cdhijklmnz",         // 18
  "cdhijknz",           // 19
  "bcefghijklmz",       // 20
  "bcefgjkz",           // 21
  "bcefgijlmnz",        // 22
  "bcefgijklnz",        // 23
  "bcefghjknz",         // 24
  "bcefghiklnz",        // 25
  "bcefghiklmnz",       // 26
  "bcefghijkz",         // 27
  "bcefghijklmnz",      // 28
  "bcefghijknz",        // 29
  "bcdeghijklmz",       // 30
  "bcdegjkz",           // 31
  "bcdegijlmnz",        // 32
  "bcdegijklnz",        // 33
  "bcdeghjknz",         // 34
  "bcdeghiklnz",        // 35
  "bcdeghiklmnz",       // 36
  "bcdeghijkz",         // 37
  "bcdeghijklmnz",      // 38
  "bcdeghijknz",        // 39
  "acdghijklmz",        // 40
  "acdgjkz",            // 41
  "acdgijlmnz",         // 42
  "acdgijklnz",         // 43
  "acdghjknz",          // 44
  "acdghiklnz",         // 45
};

typedef enum {
  BtnState_Ready = -1,
  BtnState_Start,
  BtnState_Run,
  BtnState_Pause,
  BtnState_Stop
} BtnState;

short RandomNum[45];
static BtnState btnState;
static unsigned int seed;
Adafruit_NeoPixel pixels(14, 6, NEO_GRB + NEO_KHZ800);

void Shaking_RandomNum() {
  short x, y;
  randomSeed(seed);
  for (int i = 0; i < SHAKING_NUM; i++) {
    x = random(1, 11);
    y = random(1, 11);
    if (x != y) {
      short temp = RandomNum[x];
      RandomNum[x] = RandomNum[y];
      RandomNum[y] = temp;
    }
  }
}

void Change_BtnState() {
  btnState = btnState + 1;
  delay(200);
}

void Show_Neo(int R, int G, int B, int num) {
  for (int i = 0; i < 6; i++) {
    //Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN[i], NEO_GRB + NEO_KHZ800);
    for (int j = 0; j < NUMPIXELS; j++) {
      //pixels.clear();
      //pixels.setPixelColor(MODELS[j], pixels.Color(R, G, B));
      //pixels.show();
    }
  }
}


void setup() {
  //pixels.begin();
  Serial.begin(9600);
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pixels.begin();

  pinMode(BTN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), Change_BtnState, RISING);

  btnState = BtnState_Ready;

  for (int i = 0; i < 45; i++) {
    RandomNum[i] = i + 1;
  }
}

void loop() {
  /*
    pixels.clear();

    for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();

    delay(DELAYVAL);
    }
  */
  switch (btnState) {
    case BtnState_Ready :
      Serial.println("BtnState : Ready");
      //Show_Neo(150, 0, 0);
      //Shaking_RandomNum();

      btnState = BtnState_Start;
      break;

    case BtnState_Start :
      Serial.print("BtnState : Start, Seed : ");
      Serial.println(seed);
      seed++;
      break;

    case BtnState_Run :
      Serial.println("BtnState : Run");
      for (int j = 0; j < 46; j++) {
        pixels.clear();
        for (int i = 0; i < 13; i++) {
          if (MODELS[j][i] == 'z') {
            Serial.println("fucking!!!!!!!!!!!!!!!!!!!!");
            break;
          }
          pixels.setPixelColor((int)(MODELS[j][i] - 'a'), pixels.Color(150, 0, 0));
          Serial.print("test num : ");
          Serial.println(MODELS[j][i] - 'a');
        }
        pixels.show();
        delay(1000);
      }
      break;

    case BtnState_Pause :
      Serial.println("BtnState : Pause");
      break;

    case BtnState_Stop :
      Serial.println("BtnState : Stop");
      break;

    default:
      btnState = BtnState_Ready;
      break;
  }
}
