#include <EverydayCalendar_lights.h>
#include <EverydayCalendar_touch.h>

typedef struct {
  float    x;
  float    y;
} Point;

EverydayCalendar_touch cal_touch;
EverydayCalendar_lights cal_lights;
int16_t brightness = 128;

Point ball;
Point prev;
Point vel;
Point grav;

void setup() {
  Serial.begin(9600);
  Serial.println("Sketch setup started");

  ball.x = 0;
  ball.y = 0;
  prev.x = 0;
  prev.y = 0;
  vel.x = 0.2;
  vel.y = 0.0;
  grav.x = 0;
  grav.y = 0.05;

  // Initialize LED functionality
  cal_lights.configure();
  cal_lights.setBrightness(200);
  cal_lights.begin();
  Serial.println("Gravity!");
}

void loop() {

  ball.x += vel.x;
  ball.y += vel.y;

  vel.x += grav.x;
  vel.y += grav.y;

  cal_lights.setLED((int) prev.x, (int) prev.y, false);
  cal_lights.setLED((int) ball.x, (int) ball.y, true);

  prev.x = ball.x;
  prev.y = ball.y;

  if (ball.x > 12) {
    ball.x = 12;
    vel.x *= -1;
  }

  if (ball.x < 0) {
    ball.x = 0;
    vel.x *= -1;
  }

  if (ball.y > 31) {
    ball.y = 31;
    vel.y *= -0.95;
  }

  delay(33);
}

void honeyDrip() {
  uint16_t interval_ms = 25;
  static const uint8_t monthDayOffset[12] = {0, 3, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0};
  // Turn on all LEDs one by one in the span of a few second
  for (int day = 0; day < 31; day++) {
    for (int month = 0; month < 12; month++) {
      int8_t adjustedDay = day - monthDayOffset[month];
      if (adjustedDay >= 0 ) {
        cal_lights.setLED(month, adjustedDay, true);
      }
    }
    delay(interval_ms);
    interval_ms = interval_ms + 2;
  }
}

void clearAnimation() {
  uint16_t interval_ms = 25;
  static const uint8_t monthMaxDay[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  for (int month = 11; month >= 0; month--) {
    for (int day = monthMaxDay[month] - 1; day >= 0; day--) {
      cal_lights.setLED(month, day, false);
      delay(interval_ms);
    }
  }
  cal_lights.saveLedStatesToMemory();
}
