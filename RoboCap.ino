#include <AccelStepper.h>
#include <EEPROM.h>

// Motor pin definitions
#define MOTOR_PIN_1  9      // IN1 on the ULN2003 driver 1
#define MOTOR_PIN_2  10     // IN2 on the ULN2003 driver 1
#define MOTOR_PIN_3  11     // IN3 on the ULN2003 driver 1
#define MOTOR_PIN_4  12     // IN4 on the ULN2003 driver 1
#define HALFSTEP 8
#define CAP_OPENED_POS 4477 // 180 degrees : 2048 * 94 / 43 (gears ratio)
#define CAP_CLOSED_POS 0

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper(HALFSTEP, MOTOR_PIN_1, MOTOR_PIN_3, MOTOR_PIN_2, MOTOR_PIN_4);

// Modes:
// 0 -> cap closed
// 1 -> cap open
int mode;
int nextMode;

void setup() {
  Serial.begin(9600);

  EEPROM.get(0, mode);
  if (mode < 0 || mode > 1) {
    Serial.println("Put default value in EEPROM");
    mode = 0;
    EEPROM.put(0, mode);
  }

  Serial.print("Current mode: ");
  Serial.println(mode);

  nextMode = mode;

  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(100.0);
  stepper.setSpeed(200);

  stepper.setCurrentPosition(mode == 0 ? CAP_CLOSED_POS : CAP_OPENED_POS);

  Serial.println("INIT OK");
}

void loop() {
  if (mode != nextMode && (nextMode == 0 || nextMode == 1)) {
    Serial.print("Next mode: ");
    Serial.println(nextMode);
    if (nextMode == 0) {
      stepper.runToNewPosition(CAP_CLOSED_POS);
    }
    if (nextMode == 1) {
      stepper.runToNewPosition(CAP_OPENED_POS);
    }

    mode = nextMode;
    EEPROM.put(0, mode);
    Serial.println(mode);
  }

  // Handle incoming messages
  if (Serial.available() > 0) {
    String msg = Serial.readString();
    nextMode = -1;

    if (msg[0] == '0' && mode != 0) {
      nextMode = 0;
    } else if (msg[0] == '1' && mode != 1) {
      nextMode = 1;
    } else {
      Serial.println(mode);
    }
  }

  stepper.run();
}
