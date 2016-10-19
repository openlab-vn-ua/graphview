#ifndef __CANVAS__
#define __CANVAS__

#include <OLED_I2C.h>

class Canvas
{
 public:
  Canvas();
  Canvas(OLED *);
  
  void  addCanvasUser();
  void  removeCanvasUser();

  void  beginPaint();
  void  endPaint();

  OLED* getScreen();
  void  clrScr();
  void  update();
 protected:
  OLED *screen;
  int   all_user;
  int   pain_call_count;
};
#endif
