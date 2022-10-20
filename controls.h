class Controls {
  private:
    // All arrays are in the format [Neutral, Up, Down]
    const int PINS[3] = {33, 34, 35}; // Define input pins

    const int DEBOUNCE_TIME = 0; // Debounce delay in milliseconds

    int buttonState[3] = {LOW, LOW, LOW}; // The state of each button
    int lastState[3] = {LOW, LOW, LOW}; // The last state of each button, used to only run code once when the button is pressed

    int lastPressTime = 0; // The last time a button was pressed

  public:
    const int NEUTRAL = 0;
    const int UP = 1;
    const int DOWN = 2;

    void setup_controls() {
      // Setup button pins
      pinMode(PINS[0], INPUT);
      pinMode(PINS[1], INPUT);
      pinMode(PINS[2], INPUT);
    }

    // button_pressed takes a button and returns true if it was pressed
    boolean button_pressed(int button) {
      buttonState[button] = digitalRead(PINS[button]); // Read the state of the button

      int currentTime = millis(); // Get the currest time
      int timeBetweenPresses = currentTime - lastPressTime;

      bool pressed = false; // Return variable

      if (timeBetweenPresses > DEBOUNCE_TIME) {
        // If a button was pressed, has a different state than the last time it was pressed, and
        // the time since the last press is greater than the debounce time, register a press.
        if (buttonState[button] == HIGH && buttonState[button] != lastState[button]) {
          pressed = true;
          lastPressTime = currentTime; // Set the last time a button was pressed to the current time
        } else {
          pressed = false;
        }

        lastState[button] = buttonState[button]; // Set the last state of the button to the current state
      }

      return pressed;
    }
};