#ifndef __ANIM_DRAW_BASE__
#define __ANIM_DRAW_BASE__

#include <canvas.h>
#include <OLED_I2C.h>

//--------------------------------------------

class AnimDrawBase
{
 public:
  AnimDrawBase  ();
  ~AnimDrawBase ();

  void attach     (Canvas *, uint16_t);
  void detach     ();
  bool isAttached ();
  
  virtual void setup    ();
  virtual void loopStep ();
  
  virtual void draw          (uint8_t) = 0;
  virtual void animationStep () = 0;
  
 protected:
  Canvas   *canvas;
  uint16_t  framePeriodInMs;
  uint32_t  lastDrawTs;
  bool      firstDraw;
};

//--------------------------------------------

class SquarePyramid : public AnimDrawBase
{
 public:
  SquarePyramid  ();
  SquarePyramid  (int x, int y, int sizeX, int sizeY, double angle);
  ~SquarePyramid ();
  
  virtual void draw          (uint8_t);
  virtual void animationStep ();

  void setAngleRotate (double _angle);
  void setCoordinates (int16_t, int16_t);
  void setCoordSize   (int16_t, int16_t);
 protected:
  int     x, y, sizeX, sizeY;
  double  angle;
  double  v[5][3];
};

//--------------------------------------------

class LoadingWheel : public AnimDrawBase
{
 public:
  LoadingWheel  ();
  LoadingWheel  (int, int, int, int, int);
  ~LoadingWheel ();
  
  virtual void draw          (uint8_t);
  virtual void animationStep ();

  void setCoordinates  (int, int);
  void setBigRadius    (int);
  void setLittleRadius (int);
  void setStickNumber  (int);
 protected:
  int x, y;
  int big_r, lit_r;
  int stick_numb;
  int master_skick;
  int tail_stick;

  void drawStick(int x1, int y1, int x2, int y2,
                 int stick_numb, uint8_t command);
};

//--------------------------------------------

class Pulsar : public AnimDrawBase
{
 public:
  Pulsar();
  Pulsar(int x, int y, uint16_t min_r, uint16_t max_r);

  virtual void draw(uint8_t command);
  virtual void animationStep();

  void setCoordinates (int x, int y);
  void setMinRadius   (uint16_t min_r);
  void setMaxRadius   (uint16_t max_r);
 private:
  int      x, y;
  uint16_t min_r, max_r;
  bool     clean_time;
  uint16_t curr_r;
};

//--------------------------------------------
/*
class Sphere : public AnimDrawBase
{
 public:
  Sphere();
  Sphere(int x, int y, uint16_t r, double angle);
  ~Sphere();

  virtual void draw(uint8_t command);
  virtual void animationStep();
 protected:
  int x, y, z;
  uint16_t r;
  double angle;
};
*/
//--------------------------------------------

#endif

