#define BUTTON1 0
#define BUTTON2 1
#define BUTTON3 2
#define NUMBER_OF_BUTTONS 3

void enableButton(int button);
int buttonPushed(int button);
int buttonReleased(int button);
void enableAllButtons();
void getButtonsReadyForInterrupts();
int anyButtonPushed();
int whichButtonPushed();