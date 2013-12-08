#define measure_freq 10
#include <Bridge.h>
#include <Process.h>

int mq3_analogPin = A0; // connected to the output pin of MQ3 
int led = 13;
const int buttonPin = 2;
int buttonState = 0;  

void postMeasurment(float measurmnent){
  Process p;
  char chArray[16];
  dtostrf(measurmnent, 3, 2, chArray);
  String measurmnentStr(chArray);
  String command = "curl -X POST -H \"Content-Type: application/json\" -d \'{\"device_id\":\"alkoino0\",\"raw_measurmnent\":\"" + measurmnentStr + "\"}\' http://37.233.98.53/alkomat/new";  
  p.runShellCommandAsynchronously(command);

  
  while (p.available() > 0) {
    char c = p.read();
//    Serial.print(c);
  }

//  Serial.println();
//  Serial.print(p.exitValue());
//  Serial.println();

  delay(200);
}




void setup(){
  pinMode(led, OUTPUT);     
  Bridge.begin();
//  Serial.begin(9600);
  
//  while(!Serial); // wait for a serial connection 
}



int readAlcoLevel()
{
    return analogRead(mq3_analogPin);
  
}


float blink_and_mesure(int time, int freq){
  float total = 0;
  int cycles =  time/1000 * freq;
  int cycleLenght = time/cycles;
  for(int i=0;i<cycles;++i){
    digitalWrite(led, HIGH);
    total += readAlcoLevel();
    delay(cycleLenght/2);           
    digitalWrite(led, LOW);  
    delay(cycleLenght/2);
  }
    return total/cycles; 

}



void loop()
{
  
    buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {

    digitalWrite(led, HIGH);
    delay(500);
    float measuere = blink_and_mesure(3000,measure_freq);
 //   Serial.print(measuere);    
    postMeasurment(measuere);

  } 

}
