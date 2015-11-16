
void setup() {
  Serial1.begin(57600); 
  Serial.begin(9600); 
}

void loop() {
    if (Serial1.available()) 
    {
      Serial.print((char) Serial1.read());
    }
}
