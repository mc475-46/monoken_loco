void setup(){}

void loop(){
  if(analogRead(1) > 5)
    analogWrite(11, analogRead(1));
}
