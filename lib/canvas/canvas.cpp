#include <canvas.h>

Canvas::Canvas(): all_user(0), pain_call_count(0)
{
  screen = NULL;
}

Canvas::Canvas(OLED *_screen): all_user(0), pain_call_count(0)
{
  screen = _screen;
}

void Canvas::addCanvasUser()
{
  ++all_user;
}


void Canvas::removeCanvasUser()
{
  --all_user;
}

void Canvas::beginPaint()
{
  ++pain_call_count;
}

void Canvas::endPaint()
{
  if(all_user == pain_call_count)
  {
    update();
    pain_call_count = 0;
  }
}

OLED *Canvas::getScreen()
{
  return screen;
}

void Canvas::clrScr()
{
  screen->clrScr();
}

void Canvas::update()
{
  screen->update();
}






  
