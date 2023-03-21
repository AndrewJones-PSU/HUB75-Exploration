#include <Adafruit_Protomatter.h>  // Interface for RGB Matrix

// Pins for RGB Matrix connection
uint8_t rgbPins[] = { 7, 8, 9, 10, 11, 12 };
uint8_t addrPins[] = { 17, 18, 19, 20 };
uint8_t clockPin = 14;
uint8_t latchPin = 15;
uint8_t oePin = 16;

// Initialize Protomatter
Adafruit_Protomatter matrix(64, 4, 1, rgbPins, 4, addrPins, clockPin, latchPin, oePin, true);

// cursor x position for scrolling text
int scrollTextPos = matrix.width();
char* scrollText = "Scrolling Text";
int scrollTextLen = 14;

// FPS redraw throttling
const int MAX_FPS = 45;
uint32_t prevTime = 0;

void setup() {
  Serial.begin(115200);

  // Matrix init
  ProtomatterStatus status = matrix.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)status);
  while (status != PROTOMATTER_OK) {}  // wait for begin to finish

  // Make colored bars
  // for (int i = 0; i < matrix.width(); i++) {
  //   uint8_t level = i * 256 / matrix.width();
  //   matrix.drawPixel(i, matrix.height() - 4, matrix.color565(level, 0, 0));
  //   matrix.drawPixel(i, matrix.height() - 3, matrix.color565(0, level, 0));
  //   matrix.drawPixel(i, matrix.height() - 2, matrix.color565(0, 0, level));
  //   matrix.drawPixel(i, matrix.height() - 1, matrix.color565(level, level, level));
  // }

  // Simple Shapes and Text, this is a couple of GFX library calls
  matrix.drawCircle(12, 17, 9, matrix.color565(255, 0, 0));
  matrix.drawRect(14, 9, 17, 17, matrix.color565(0, 255, 0));
  matrix.drawTriangle(32, 9, 41, 27, 23, 27, matrix.color565(0, 0, 255));
  matrix.show();
  matrix.setTextWrap(false);
  matrix.setTextColor(matrix.color565(255, 255, 255), 0);
}

void loop() {
  // limit to MAX_FPS
  uint32_t t;
  while (((t = micros()) - prevTime) < (1000000L / MAX_FPS))
    ;
  prevTime = t;
  // update scroll text
  matrix.setCursor(scrollTextPos, 0);
  matrix.print(scrollText);
  scrollTextPos--;
  if (scrollTextPos < 0 - (scrollTextLen * 6))
    scrollTextPos = matrix.width();
  matrix.show();
}
