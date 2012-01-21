#include <ECGFilters.h>

qrs_filter filter = qrs_filter();

void setup() {
  Serial.begin(115200);
}

void loop() {
  int sensorValue = analogRead(A0);
  
  return_value result = filter.step(sensorValue);
  
  if (result.respiration_phase > -1)
  {
    Serial.print(result.respiration_phase, BYTE);
  }
  
  delayMicroseconds(5000);
}
