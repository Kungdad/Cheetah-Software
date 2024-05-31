#include "GameController.h"
#include <QtCore/QObject>
#include <QtGamepad/QGamepad>
GameController::GameController(QObject *parent) : QObject(parent) {
  findNewController();
}

void GameController::findNewController() {
  delete _qGamepad;
  _qGamepad = nullptr;  // in case this doesn't work!

  printf("[Gamepad] Searching for gamepads, please ignore \"Device discovery cannot open device\" errors\n");
  auto gamepadList = QGamepadManager::instance()->connectedGamepads();
  printf("[Gamepad] Done searching for gamepads.\n");
  if (gamepadList.empty()) {
    printf(
        "[ERROR: GameController] No controller was connected! All joystick "
        "commands will be zero!\n");
  } else {
    if (gamepadList.size() > 1) {
      printf(
          "[ERROR: GameController] There are %d joysticks connected.  Using "
          "the first one.\n",
          gamepadList.size());
    } else {
      printf("[GameController] Found 1 joystick\n");
    }

    _qGamepad = new QGamepad(*gamepadList.begin());
  }
}

void GameController::updateGamepadCommand(GamepadCommand &gamepadCommand) {
  if (_qGamepad) {
    gamepadCommand.leftBumper = _qGamepad->buttonL1();
    gamepadCommand.rightBumper = _qGamepad->buttonR1();
    gamepadCommand.leftTriggerButton = _qGamepad->buttonL2() != 0.;
    gamepadCommand.rightTriggerButton = _qGamepad->buttonR2() != 0.;
    gamepadCommand.back = _qGamepad->buttonSelect();
    gamepadCommand.start = _qGamepad->buttonStart();
    gamepadCommand.a = _qGamepad->buttonA();
    gamepadCommand.b = _qGamepad->buttonB();
    gamepadCommand.x = _qGamepad->buttonX();
    gamepadCommand.y = _qGamepad->buttonY();
    gamepadCommand.leftStickButton = _qGamepad->buttonL3();
    gamepadCommand.rightStickButton = _qGamepad->buttonR3();
    gamepadCommand.leftTriggerAnalog = (float)_qGamepad->buttonL2();
    gamepadCommand.rightTriggerAnalog = (float)_qGamepad->buttonR2();
    gamepadCommand.leftStickAnalog =
        Vec2<float>(_qGamepad->axisLeftX(), -_qGamepad->axisLeftY());
    gamepadCommand.rightStickAnalog =
        Vec2<float>(_qGamepad->axisRightX(), -_qGamepad->axisRightY());
  } else {
    //printf("no joystick, plz use keyboard instead.\n");
    //gamepadCommand.zero();  // no joystick, return all zeros
  }

  // printf("%s\n", gamepadCommand.toString().c_str());
}

GameController::~GameController() { delete _qGamepad; }
