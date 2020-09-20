#include <EverydayCalendar_lights.h>
#include <EverydayCalendar_touch.h>


const int cols = 12;
const int rows = 31;

bool grid[cols * rows];
bool next[cols * rows];

bool interaction = false;
unsigned long now;


EverydayCalendar_touch cal_touch;
EverydayCalendar_lights cal_lights;

void setup() {
  //Serial.begin(9600);
  //Serial.println("Sketch setup started");
  //randomSeed(analogRead(0));

  // Initialize LED functionality
  cal_lights.configure();
  cal_lights.setBrightness(200);
  cal_lights.begin();
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      int val = random(2);
      if (val == 1) grid[i + j * cols] = true;
      else grid[i + j * cols] = false;
      cal_lights.setLED(i, j, grid[i + j * cols]);
    }
  }
  //Serial.println("Sketch setup complete");

  //Serial.println("Starting touch configuration");
  cal_touch.configure();
  cal_touch.begin();






}

void loop() {

  // handle touch interaction
  bool touch = cal_touch.scanForTouch();
  if (touch) {
    // TODO: Debounce this toggling!
    bool state = grid[cal_touch.x + cal_touch.y * cols];
    grid[cal_touch.x + cal_touch.y * cols] = !state;
    cal_lights.setLED(cal_touch.x, cal_touch.y, !state);
    now = millis();
    interaction = true;
  } else if (interaction) {
      interaction = millis() - now < 5000;
  } else {
    // Compute next based on grid
    for (int i = 0; i < cols; i++) {
      for (int j = 0; j < rows; j++) {
        bool state = grid[i + j * cols];

        // Count live neighbors!
        int neighbors = 0;
        for (int x = -1; x < 2; x++) {
          for (int y = -1; y < 2; y++) {
            int col = (i + x + cols) % cols;
            int row = (j + y + rows) % rows;
            if (grid[col + row * cols]) {
              neighbors++;
            }
          }
        }
        if (state) {
          neighbors--;
        }

        if (!state && neighbors == 3) {
          next[i + j * cols] = true;
        } else if (state && (neighbors < 2 || neighbors > 3)) {
          next[i + j * cols] = false;
        } else {
          next[i + j * cols] = state;
        }

      }
    }


 // memcpy(grid,next,rows*cols);
    for (int i = 0; i < cols; i++) {
      for (int j = 0; j < rows; j++) {
        cal_lights.setLED(i, j, grid[i + j * cols]);
        grid[i + j * cols] = next[i + j * cols];
      }
    }
  }
}
