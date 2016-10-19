#include <AnimDrawBase.h>

//-------------------------------------------

void rotate(int x, int y, double angle,
            int ref_point_x, int ref_point_y,
            int &new_x, int &new_y)
{
  double cosA = cos(angle);
  double sinA = sin(angle);

  int x0 = x - ref_point_x;
  int y0 = y - ref_point_y;
  
  new_x = ref_point_x + x0*cosA - y0*sinA;
  new_y = ref_point_y + x0*sinA + y0*cosA;
}

//-------------------------------------------

AnimDrawBase::AnimDrawBase(): canvas(NULL)
{
  framePeriodInMs = 0;
  lastDrawTs = 0;
  firstDraw = false;
}

AnimDrawBase::~AnimDrawBase()
{
  
}

void AnimDrawBase::attach(Canvas *canvas, uint16_t framePeriodInMs)
{
  this->canvas = canvas;
  this->framePeriodInMs = framePeriodInMs;
  lastDrawTs = millis() - framePeriodInMs;
  firstDraw = false;
  canvas->addCanvasUser();
}

void AnimDrawBase::detach()
{
  canvas->removeCanvasUser();
  canvas = NULL;
  framePeriodInMs = 0;
  lastDrawTs = 0;
  firstDraw = false;
}

bool AnimDrawBase::isAttached()
{
  return (canvas != NULL) ? true : false;
}

void AnimDrawBase::setup()
{
  
}

void AnimDrawBase::loopStep()
{
  if (isAttached())
 {
    canvas->beginPaint();
    
    if((millis() - lastDrawTs) >= framePeriodInMs)
    {
      Serial.print("time: ");
      Serial.println(millis());
      lastDrawTs = millis();
      if(!firstDraw)
      {
        draw(1);
        firstDraw = true;
      }
      else
      {
        draw(0);
        animationStep();
        draw(1);
      }
    }
    
    canvas->endPaint();
  }
}

//----------------------------------------------

SquarePyramid::SquarePyramid(): AnimDrawBase()
{
  angle = .1;
  x = 0; y = 0;
  sizeX = 0; sizeY = 0;
  v[0][0] =  0.5; v[0][1] = -0.5; v[0][2] =  0.5;
  v[1][0] =  0.5; v[1][1] = -0.5; v[1][2] = -0.5;
  v[2][0] = -0.5; v[2][1] = -0.5; v[2][2] = -0.5;
  v[3][0] = -0.5; v[3][1] = -0.5; v[3][2] =  0.5;
  v[4][0] =    0; v[4][1] =  0.5; v[4][2] =    0;
}

SquarePyramid::SquarePyramid(int x, int y,
                             int sizeX, int sizeY,
                             double angle): AnimDrawBase()
{
  this->x = x; this->y = y;
  this->sizeX = sizeX; this->sizeY = sizeY;
  this->angle = angle;
  v[0][0] =  0.5; v[0][1] = -0.5; v[0][2] =  0.5;
  v[1][0] =  0.5; v[1][1] = -0.5; v[1][2] = -0.5;
  v[2][0] = -0.5; v[2][1] = -0.5; v[2][2] = -0.5;
  v[3][0] = -0.5; v[3][1] = -0.5; v[3][2] =  0.5;
  v[4][0] =    0; v[4][1] =  0.5; v[4][2] =    0;
}

SquarePyramid::~SquarePyramid()
{
  
}

void SquarePyramid::draw(uint8_t command)
{
  OLED *screen = canvas->getScreen();
  void (OLED::*drawFunc)(int, int, int, int) = NULL;
  if     (command == 0)
  {
    drawFunc = &OLED::clrLine;
  }
  else if(command == 1)
  {
    drawFunc = &OLED::drawLine;
  }
  
  (screen->*drawFunc)(v[0][0]*sizeX + x, -v[0][1]*sizeY + y, v[1][0]*sizeX + x, -v[1][1]*sizeY + y);
  (screen->*drawFunc)(v[1][0]*sizeX + x, -v[1][1]*sizeY + y, v[2][0]*sizeX + x, -v[2][1]*sizeY + y);
  (screen->*drawFunc)(v[2][0]*sizeX + x, -v[2][1]*sizeY + y, v[3][0]*sizeX + x, -v[3][1]*sizeY + y);
  (screen->*drawFunc)(v[3][0]*sizeX + x, -v[3][1]*sizeY + y, v[0][0]*sizeX + x, -v[0][1]*sizeY + y);
  
  (screen->*drawFunc)(v[0][0]*sizeX + x, -v[0][1]*sizeY + y, v[4][0]*sizeX + x, -v[4][1]*sizeY + y);
  (screen->*drawFunc)(v[1][0]*sizeX + x, -v[1][1]*sizeY + y, v[4][0]*sizeX + x, -v[4][1]*sizeY + y);
  (screen->*drawFunc)(v[2][0]*sizeX + x, -v[2][1]*sizeY + y, v[4][0]*sizeX + x, -v[4][1]*sizeY + y);
  (screen->*drawFunc)(v[3][0]*sizeX + x, -v[3][1]*sizeY + y, v[4][0]*sizeX + x, -v[4][1]*sizeY + y);
}

void SquarePyramid::animationStep()
{
  int cyc_numb = sizeof(v)/sizeof(v[0]);
  
  double cosa = cos(angle);
  double sina = sin(angle);
  
  //oz rot
  for(int i = 0; i < cyc_numb; ++i){
    double newX = v[i][0] * cosa - v[i][1] * sina;
    double newY = v[i][0] * sina + v[i][1] * cosa;
    v[i][0] = newX;
    v[i][1] = newY;
  }
  //oy rot
  for(int i = 0; i < cyc_numb; ++i){
      double newX = v[i][0] * cosa + v[i][2] * sina;
      double newY = -v[i][0] * sina + v[i][2] * cosa;
      v[i][0] = newX;
      v[i][2] = newY;
  }
  //ox rot
  for(int i = 0; i < cyc_numb; ++i){
    double newX = v[i][1] * cosa + v[i][2] * sina;
    double newY = -v[i][1] * sina + v[i][2] * cosa;
    v[i][1] = newX;
    v[i][2] = newY;
  }
}

void SquarePyramid::setAngleRotate (double angle)
{
  this->angle = angle;
}

void SquarePyramid::setCoordinates (int16_t x, int16_t y)
{
  this->x = x;
  this->y = y;
}

void SquarePyramid::setCoordSize   (int16_t sizeX, int16_t sizeY)
{
  this->sizeX = sizeX;
  this->sizeY = sizeY;
}

//----------------------------------------------

LoadingWheel::LoadingWheel  (): AnimDrawBase()
{
  x = 0; y = 0;
  big_r = 0; lit_r = 0;
  stick_numb = 0;
  master_skick = 0;
}

LoadingWheel::LoadingWheel  (int x, int y, int big_r, int lit_r, int stick_numb): AnimDrawBase()
{
  this->x = x; this->y = y;
  this->big_r = big_r; this->lit_r = lit_r;
  this->stick_numb = stick_numb;
  master_skick = stick_numb * 0.8;
  tail_stick   = 0;
}

LoadingWheel::~LoadingWheel ()
{
  
}

void LoadingWheel::draw(uint8_t command)
{
  double angle_rotate = 2*PI/stick_numb;
  int    new_lit_x = x;
  int    new_lit_y = y + lit_r;
  int    new_big_x = x;
  int    new_big_y = y + big_r;

  for(int i = 0; i < stick_numb; ++i)
  {
    rotate(x, y + lit_r, i*angle_rotate, x, y, new_lit_x, new_lit_y);
    rotate(x, y + big_r, i*angle_rotate, x, y, new_big_x, new_big_y);
    
    if(master_skick > tail_stick)
    {
      if(i >= tail_stick && i <= master_skick)
      {
        drawStick(new_lit_x, new_lit_y,
                  new_big_x, new_big_y,
                  i, command);
      }
    }
    else
    {
      if(i >= tail_stick || i <= master_skick)
      {
        drawStick(new_lit_x, new_lit_y,
                  new_big_x, new_big_y,
                  i, command);
      }
    }
  }
}

void LoadingWheel::animationStep ()
{
  ++master_skick;
  if(master_skick == stick_numb)
  {
    master_skick = 0;
  }
  
  ++tail_stick;
  if(tail_stick == stick_numb)
  {
    tail_stick = 0;
  }
}

void LoadingWheel::setCoordinates  (int x, int y)
{
  this->x = x; this->y = y;
}

void LoadingWheel::setBigRadius    (int big_r)
{
  this->big_r = big_r;
}

void LoadingWheel::setLittleRadius (int lit_r)
{
    this->lit_r = lit_r;
}

void LoadingWheel::setStickNumber  (int stick_numb)
{
  this->stick_numb = stick_numb;
}

//protected
void LoadingWheel::drawStick(int x1, int y1, int x2, int y2,
                             int stick_numb, uint8_t command)
{
  OLED    *screen = canvas->getScreen();
  void (OLED::*drawFunc)(int, int, int, int) = NULL;
  if     (command == 0)
  {
    drawFunc = &OLED::clrLine;
  }
  else if(command == 1)
  {
    drawFunc = &OLED::drawLine;
  }

  if(stick_numb == tail_stick && command != 0)
  {
    screen->drawDashLine(x1, y1, x2, y2, 1, 3);
  }
  else if(((tail_stick == (this->stick_numb - 1) && stick_numb == 0) ||
           (tail_stick != (this->stick_numb - 1) && stick_numb == (tail_stick + 1)))
          && command != 0 )
  {
    screen->drawDashLine(x1, y1, x2, y2, 1, 2);
  }
  else
  {
    (screen->*drawFunc)(x1, y1, x2, y2);
  }
}

//----------------------------------------------

Pulsar::Pulsar(): AnimDrawBase()
{
  x = 0; y = 0;
  min_r = 0; max_r = 0;
  clean_time = false;
}

Pulsar::Pulsar(int x, int y, uint16_t min_r, uint16_t max_r): AnimDrawBase()
{
  this->x = x;         this->y = y;
  this->min_r = min_r; this->max_r = max_r;
  clean_time = false;
  curr_r = min_r;
}

void Pulsar::draw(uint8_t command)
{
  OLED *screen = canvas->getScreen();
  
  if     (command == 0)
  {
    //screen->clrCircle(x, y, curr_r);
  }
  else if(command == 1)
  {
    if(clean_time)
    {
      screen->clrCircle(x, y, curr_r);
    }
    else
    {
      screen->drawCircle(x, y, curr_r); 
    }   
  }
}

void Pulsar::animationStep()
{
  if(curr_r < max_r && !clean_time)
  {
    ++curr_r;
  }
  else if(curr_r == max_r && !clean_time)
  {
    clean_time = !clean_time;
  }
  else if(curr_r > min_r && clean_time)
  {
    --curr_r;
  }
  else if(curr_r == min_r && clean_time)
  {
    clean_time = !clean_time;
  }
}

void Pulsar::setCoordinates (int x, int y)
{
  this->x = x;
  this->y = y;
}

void Pulsar::setMinRadius   (uint16_t min_r)
{
  this->min_r = min_r;
}

void Pulsar::setMaxRadius   (uint16_t max_r)
{
  this->max_r = max_r;
}

//----------------------------------------------
/*
Sphere::Sphere(): AnimDrawBase()
{
  x = 0; y = 0; z = 0;
  r = 0; angle = 0;
}

Sphere::Sphere(int x, int y, uint16_t r, double angle)
{
  this->x = x; this->y = y; this->z = 0;
  this->r = r; this->angle = angle;
}

Sphere::~Sphere()
{
  
}

void Sphere::draw(uint8_t command)
{
  OLED *screen = canvas->getScreen();
  void (OLED::*drawFunc)(int, int, int, int) = NULL;
  if(command == 0)
  {
    drawFunc = &OLED::clrLine;
  }
  else
  {
    drawFunc = &OLED::drawLine;
  }

  double angle_1 = 0; // 0..2*PI
  double angle_1_step = 2*PI/10;
  double angle_2 = 0; // 0..PI
  double angle_2_step = PI/10;

  for(angle_2 = 0; angle_2 < PI; angle_2 += angle_2_step)
  {
    for(angle_1 = 0; angle_1 < 2*PI; angle_1 += angle_1_step)
    {
      
    }
  }
}

void Sphere::animationStep()
{
  
}
*/
//----------------------------------------------
