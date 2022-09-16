
#include <HX711.h>


#define calibration_factor -7050.0
#define DOUT 3
#define CLK 2
HX711 scale(DOUT, CLK);

void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   Serial.println("HX 711 test");
   scale.set_scale(calibration_factor);
   scale.tare();
   Serial.println("Readings:");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Readings:");
  Serial.print(scale.get_units(),1);
  Serial.println("lbs");
  Serial.println("");
}
