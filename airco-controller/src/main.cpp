#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);


  // Test function
  int result = myFunction(2, 3);
  Serial.println(result);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World");
  delay(1000);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}