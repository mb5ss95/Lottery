#include <Adafruit_NeoPixel.h>

#define BTN_PIN         2
#define NUMPIXELS      14
#define SHAKING_NUM   150
#define DELAYVAL      500

const int NEO_PIN[] = {6, 7, 8, 9, 10, 11};
const char *MODELS[46] = {
  "abcdefhijklm",      // 0
  "abcdefjk",          // 1
  "abcdefijlmn",       // 2
  "abcdefijkln",       // 3
  "abcdefhjkn",        // 4
  "abcdefhikln",       // 5
  "abcdefhiklmn",      // 6
  "abcdefhijk",        // 7
  "abcdefhijklmn",     // 8
  "abcdefhijkn",       // 9
  "cdhijklm",          // 10
  "cdjk",              // 11
  "cdijlmn",           // 12
  "cdijkln",           // 13
  "cdhjkn",            // 14
  "cdhikln",           // 15
  "cdhiklmn",          // 16
  "cdhijk",            // 17
  "cdhijklmn",         // 18
  "cdhijkn",           // 19
  "bcefghijklm",       // 20
  "bcefgjk",           // 21
  "bcefgijlmn",        // 22
  "bcefgijkln",        // 23
  "bcefghjkn",         // 24
  "bcefghikln",        // 25
  "bcefghiklmn",       // 26
  "bcefghijk",         // 27
  "bcefghijklmn",      // 28
  "bcefghijkn",        // 29
  "bcdeghijklm",       // 30
  "bcdegjk",           // 31
  "bcdegijlmn",        // 32
  "bcdegijkln",        // 33
  "bcdeghjkn",         // 34
  "bcdeghikln",        // 35
  "bcdeghiklmn",       // 36
  "bcdeghijk",         // 37
  "bcdeghijklmn",      // 38
  "bcdeghijkn",        // 39
  "acdghijklm",        // 40
  "acdgjk",            // 41
  "acdgijlmn",         // 42
  "acdgijkln",         // 43
  "acdghjkn",          // 44
  "acdghikln",         // 45
};

typedef enum {
  BtnState_Ready = -1,
  BtnState_Start,
  BtnState_Run,
  BtnState_Pause,
} BtnState;

short ResultNum[6];
static short RandomNum[45];
static BtnState btnState;
static unsigned int seed;

void Shaking_RandomNum() {
  short x, y;

  randomSeed(seed);
  for (int i = 0; i < SHAKING_NUM; i++) {
    x = random(1, 46);
    y = random(1, 46);
    if (x != y) {
      short temp = RandomNum[x];
      RandomNum[x] = RandomNum[y];
      RandomNum[y] = temp;
    }
  }
  for (int i = 0; i < 6; i++) {
    ResultNum[i] = RandomNum[i + 1];
  }
}

void Sort_ResultNum() {
  for (int i = 0; i < 5; i++) {
    for (int j = i + 1; j < 6; j++) {
      if (ResultNum[i] > ResultNum[j]) {
        short temp = ResultNum[i];
        ResultNum[i] = ResultNum[j];
        ResultNum[j] = temp;
      }
    }
  }
}

void Change_BtnState() {
  btnState = btnState + 1;
  delay(200);
}

void Neo_Init() {
  for (int i = 0; i < 6; i++) {
    Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN[i], NEO_GRB + NEO_KHZ800);
    pixels.begin();
    pixels.clear();
    for (int j = 0; j < NUMPIXELS; j++) {
      if (MODELS[i][j] == '\0') {
        break;
      }
      pixels.setPixelColor(MODELS[0][j] - 'a', pixels.Color(150, 0, 0));
    }
    delay(10);
    pixels.show();
  }
}

void Neo_Run() {
  for (int i = 0; i < 6; i++) {
    Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN[i], NEO_GRB + NEO_KHZ800);
    pixels.begin();
    pixels.clear();
    for (int j = 0; j < NUMPIXELS; j++) {
      if (MODELS[i][j] == '\0') {
        break;
      }
      pixels.setPixelColor(MODELS[ResultNum[i]][j] - 'a', pixels.Color(150, 0, 0));
    }
    delay(20);
    pixels.show();
  }
}


void setup() {
  Serial.begin(9600);

  pinMode(BTN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), Change_BtnState, RISING);

  btnState = BtnState_Ready;

  for (int i = 0; i < 45; i++) {
    RandomNum[i] = i + 1;
  }
}

void loop() {
  switch (btnState) {
    case BtnState_Ready :
      Serial.println("BtnState : Ready");
      Neo_Init();
      btnState = BtnState_Start;
      break;

    case BtnState_Start :
      Serial.print("BtnState : Start, Seed : ");
      Serial.println(seed);
      seed++;
      break;

    case BtnState_Run :
      Serial.println("BtnState : Run");
      Shaking_RandomNum();
      Sort_ResultNum();
      Neo_Run();
      break;

    case BtnState_Pause :
      Serial.println("BtnState : Pause");
      break;

    default:
      btnState = BtnState_Ready;
      break;
  }
}
