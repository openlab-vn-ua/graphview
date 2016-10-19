#include <OLED_I2C.h>
#include <canvas.h>
#include <AnimDrawBase.h>
#include <data.h>

extern uint8_t SmallFont[];

OLED   myOLED(SDA, SCL, 8);
Canvas canvas(&myOLED);

SquarePyramid  piramid (63, 40, 23, 23, 0.1);
LoadingWheel   wheel   (107, 25, 9, 20, 12);
Pulsar         pulsar  (25, 25, 2, 20);

//-----------------------------------------------------

void setup(){
  myOLED.begin();
  myOLED.setFont(SmallFont);
  
  piramid.setup();
  pulsar.setup();
  wheel.setup();
  
  piramid.attach (&canvas, 50);
  pulsar.attach  (&canvas, 20);
  wheel.attach   (&canvas, 100);
}

void loop(){
  piramid.loopStep();
  wheel.loopStep();
  pulsar.loopStep();
}
