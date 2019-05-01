//#define DEBUG
const int trig1_pin = A1;
const int trig2_pin = A4;
const int trig3_pin = 8;
const int echo1_pin = A2;
const int echo2_pin = A5;
const int echo3_pin = 12;
const int Motor1_Pin1 = 3;
const int Motor1_Pin2 = 11;
const int Motor2_Pin1 = 9;
const int Motor2_Pin2 = 10;
const int buzzer_Pin = 13;
unsigned int  front,right,left;
long duration, dist1,dist2,dist3;
int sensorValue;
int referenceValue = 35;
bool ReadyForNextDetection = true;
int ReadyForNextDetectionCounter;

void motor1_run (int Direction, int Speed);
void motor2_run (int Direction, int Speed);
int GetRandom(int Min, int Max);

void setup(){
  Serial.begin(9600);
  
  pinMode(Motor1_Pin1,OUTPUT);
  pinMode(Motor1_Pin2,OUTPUT);
  pinMode(Motor2_Pin1,OUTPUT);
  pinMode(Motor2_Pin2,OUTPUT);
  
  pinMode(buzzer_Pin,OUTPUT);
  
  pinMode(trig1_pin,OUTPUT);
  pinMode(trig2_pin,OUTPUT);
  pinMode(trig3_pin,OUTPUT);
  pinMode(echo1_pin,INPUT);
  pinMode(echo2_pin,INPUT);
  pinMode(echo3_pin,INPUT);
}

void loop(){  
  int sensorValue = analogRead(A0);
#ifdef DEBUG
  Serial.print("sensor value= ");
  Serial.print(sensorValue);
  
  Serial.print("\t renk= ");
  if(sensorValue < referenceValue){
    Serial.println("black");
 }
  else{
    Serial.println("white");
  }
  delay(5); 
#endif


  for(int i=1; i<3; i++){
   if(i=1){             //left sensor
    digitalWrite(trig1_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trig1_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig1_pin, LOW);
    int pulse = pulseIn(echo1_pin, HIGH, 1164);
      if(pulse == 0){
        dist1=50;
      }
      else{
        dist1 = pulse/58.2;
      }
   }

     if(i=2){                    //front sensor
      digitalWrite(trig2_pin, LOW);
      delayMicroseconds(2);
      digitalWrite(trig2_pin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig2_pin, LOW);
      int pulse = pulseIn(echo2_pin, HIGH, 1164);
      if(pulse == 0){
        dist2=50;
      }
      else{
        dist2 = pulse/58.2;
      }

     }
  
     if(i=3){                  //   right sensor
      digitalWrite(trig3_pin, LOW);
      delayMicroseconds(2);
      digitalWrite(trig3_pin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig3_pin, LOW);
      int pulse = pulseIn(echo3_pin, HIGH, 1164);
      if(pulse == 0){
        dist3=50;
      }
      else{
        dist3 = pulse/58.2;
      }
     }
  }
    if(ReadyForNextDetection == true){
      if(sensorValue < referenceValue){
        motor1_run(1, 0);
        motor2_run(1, 0);
        digitalWrite(buzzer_Pin, HIGH);
        delay(2000);
        digitalWrite(buzzer_Pin,LOW);
        ReadyForNextDetection = false;
      }
    }
    else{
      if(sensorValue >= referenceValue){
        ReadyForNextDetectionCounter++;
        if(ReadyForNextDetectionCounter > 2){
          ReadyForNextDetectionCounter = 0;
          ReadyForNextDetection = true;
        }
      }
    }
    
    if(dist2 <= 5){
        motor1_run(1, 0);
        motor2_run(1, 0);
        delay(1000);
        if(dist1 <= 5 && dist3 <=5){
         //if both sides are close, full turn
          motor1_run(1,95);
          motor2_run(0,95);
          delay(1500);
        }
        else if(dist1<=10){
          motor2_run(1,95);
          motor1_run(0,95);
          delay(GetRandom(250, 1000));
        }
        else{
          motor2_run(0,95);
          motor1_run(1,95);
          delay(GetRandom(250, 1000));
            //rand() % 50 >>> a random number to 50.        
        }
    } 
    else if(dist1 <= 2){
       motor1_run(1, 0);
       motor2_run(1, 0);
       delay(1000);
       motor2_run(1,95);
       motor1_run(0,95);
       delay(250);
    }
    else if(dist3 <= 2){
      motor1_run(1, 0);
      motor2_run(1, 0);
      delay(1000);
      motor2_run(0,95);
      motor1_run(1,95);
      delay(250);
    }
    else{
      motor1_run(1, 95);
      motor2_run(1, 95);
    }

// after the for loop send the values to the serial monitor before restart
//  Serial.print("left sensor: ");
//  Serial.print(dist1);
//  Serial.print("   front sensor: ");
//  Serial.print(dist2);
//  Serial.print("   right sensor: ");
//  Serial.println(dist3);

}
////--------------------------------------------
// if there is an obstacle front, check left and right. If both are close, full turn (tank turn)
// 

void motor1_run (int Direction, int Speed){
  if(Direction == 1){ 
    digitalWrite(Motor1_Pin2,LOW);
    analogWrite(Motor1_Pin1, Speed);
  }
  else{
    digitalWrite(Motor1_Pin1,LOW);
    analogWrite(Motor1_Pin2, Speed);
  }
}


void motor2_run (int Direction, int Speed){
  if(Direction == 1){ 
    digitalWrite(Motor2_Pin2,LOW);
    analogWrite(Motor2_Pin1, Speed);
  }
  else{
    digitalWrite(Motor2_Pin1,LOW);
    analogWrite(Motor2_Pin2, Speed);
  }
}

int GetRandom(int Min, int Max){
  srand(millis());
  return (rand() % (Max - Min)) + Min;
}

