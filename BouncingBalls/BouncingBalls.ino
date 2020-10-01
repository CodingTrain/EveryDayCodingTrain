#include <EverydayCalendar_lights.h>
#include <EverydayCalendar_touch.h>

typedef struct {
  float    x;
  float    y;
} Point;

typedef struct {
  Point pos;
  Point pre;
  Point vel;
  Point acc;
} Particle;

const int cols = 12;
const int rows = 31;

const int TOTAL = 15;
Particle particles[TOTAL];
int pIndex = 0;

EverydayCalendar_touch cal_touch;
EverydayCalendar_lights cal_lights;
int16_t brightness = 128;

void setup() {
  //Serial.begin(9600);
  //Serial.println("Sketch setup started");

  for (int i = 0; i < TOTAL; i++) {
    Particle *p = &particles[i];
    p->pos.x = cols * (random(100) / 100.0);
    p->pos.y = 4 * (random(100) / 100.0);
    p->pre.x = p->pos.x;
    p->pre.y = p->pos.y;
    p->vel.x = -0.5 + (random(100) / 100.0);
    p->vel.y = 0.0;
    p->acc.x = 0;
    p->acc.y = 0.05;
  }

  // Initialize LED functionality
  cal_lights.configure();
  cal_lights.setBrightness(200);
  cal_lights.begin();

  //Serial.println("Starting touch configuration");
  cal_touch.configure();
  cal_touch.begin();


}

void loop() {
  bool touch = cal_touch.scanForTouch();
  if (touch) {
    Particle par;
    par.pos.x = cal_touch.x;
    par.pos.y = cal_touch.y;
    par.pre.x = cal_touch.x;
    par.pre.y = cal_touch.y;
    par.vel.x = -0.5 + (random(100) / 100.0);
    par.vel.y = 0.0;
    par.acc.x = 0;
    par.acc.y = 0.05;
    particles[pIndex] = par;
    pIndex = (pIndex + 1) % TOTAL;
  } else {
    int endP = min(pIndex, TOTAL - 1);
    for (int i = 0; i < endP; i++) {
      Particle *p = &particles[i];
      cal_lights.setLED((int) p->pre.x, (int) p->pre.y, false);
    }
    for (int i = 0; i < endP; i++) {
      Particle *p = &particles[i];
      cal_lights.setLED((int) p->pos.x, (int) p->pos.y, true);
      p->pre.x = p->pos.x;
      p->pre.y = p->pos.y;
      p->pos.x += p->vel.x;
      p->pos.y += p->vel.y;
      p->vel.x += p->acc.x;
      p->vel.y += p->acc.y;
      if (p->pos.x > 12) {
        p->pos.x = 12;
        p->vel.x *= -1;
      }

      if (p->pos.x < 0) {
        p->pos.x = 0;
        p->vel.x *= -1;
      }

      if (p->pos.y > 31) {
        p->pos.y = 31;
        p->vel.y *= -0.9;
      }
    }
  }
  // delay(33);
}
