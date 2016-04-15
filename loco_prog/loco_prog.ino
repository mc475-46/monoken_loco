#define ATSSW 4
#define ATSBUZ 5
#define ATSBEACON 6
#define TRANSF 7
#define TRANSB 8
#define RLF 2
#define RLB 3
#define PWMF 9
#define PWMB 10
#define MASCON 0
#define REF 1

int ref,mascon,throttle;
boolean buzState;
boolean atsState;

void setup()
{
  pinMode(ATSSW,INPUT_PULLUP);
  pinMode(ATSBUZ,OUTPUT);
  pinMode(ATSBEACON,INPUT_PULLUP);
  pinMode(RLF,OUTPUT);
  pinMode(RLB,OUTPUT);
  pinMode(PWMF,OUTPUT);
  pinMode(PWMB,OUTPUT);
  pinMode(TRANSF,INPUT);
  pinMode(TRANSB,INPUT);

  analogWrite(PWMF,0);
  analogWrite(PWMB,0);
  digitalWrite(ATSBUZ,LOW);
  digitalWrite(RLF,LOW);
  digitalWrite(RLB,LOW);

  ref=1023;
  buzState=false;
  atsState=false;
}

void loop()
{
  mascon=analogRead(MASCON);
  delay(50);
  if(mascon<40)
    mascon=0;
  throttle=mascon/4;
  
  //if(throttle>255)
  //  throttle=255;

  if(digitalRead(TRANSF)==LOW){
    digitalWrite(RLF,HIGH);
    digitalWrite(RLB,LOW);
    analogWrite(PWMF,throttle);
    analogWrite(PWMB,0);
  }
  else if(digitalRead(TRANSB)==LOW){
    digitalWrite(RLB,HIGH);
    digitalWrite(RLF,LOW);
    analogWrite(PWMF,0);
    analogWrite(PWMB,throttle);
  }
  else {
    digitalWrite(RLF,LOW);
    digitalWrite(RLB,LOW);
    analogWrite(PWMF,0);
    analogWrite(PWMB,0);
  }

  if(digitalRead(ATSBEACON)==LOW)
    atsState=true;
  
  if(digitalRead(ATSSW)!=LOW){
    delay(20);
    if(digitalRead(ATSSW)!=LOW){
      if(atsState){
        buzState=true;
        soundBuzzer(ATSBUZ,buzState);
      }
    }
  }else{
    atsState=false;
    soundBuzzer(ATSBUZ,false);
  }
}

void soundBuzzer(int pin,byte state)
{
  if(state)
    tone(pin,500);
  else
    noTone(pin);
}

