#include <PS4Controller.h>

void setup() {
  Serial.begin(115200);
  PS4.begin("04:33:F7:64:1D:44");  // Replace with your ESP32's MAC address
  Serial.println("Ready to connect PS4 controller");
}

void loop() {
  if (PS4.isConnected()) {
    int l2 = PS4.L2Value();  // 0–255
    int r2 = PS4.R2Value();  // 0–255
    bool boost_button = PS4.Cross();

    // Serial.print("L2: ");
    // Serial.print(l2);
    // Serial.print("\tR2: ");
    // Serial.println(r2);


    Serial.print("\boost_button: ");
    Serial.println(boost_button);
    delay(100);
  }
}
