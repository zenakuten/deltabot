
#include <Delta.h>
#include <Servo.h>

const int baud = 19200;
const int BUFSIZE = 256;
char cmd[BUFSIZE];
char *pcmd = &cmd[0];
boolean done = false;

            
 const float BOT_E = 92.0;     // end effector
 const float BOT_F = 25.0;     // base
 const float BOT_RF = 60.0;    //upper joint length
 const float BOT_RE = 185.0;   //parallelogram joint length

const int ANGLE_MIN = 30; //safety 
const int ANGLE_MAX = 120;  //safety
 
Delta dt(BOT_E,BOT_F,BOT_RF, BOT_RE);

const int xServoPin = 8, 
          yServoPin = 9, 
          zServoPin = 10;

Servo ServoX, ServoY, ServoZ;


void setup() {

  ServoX.attach(xServoPin);
  ServoY.attach(yServoPin);
  ServoZ.attach(zServoPin);

  cmd[0] = 0;
  Serial.begin(baud);   
}

void serialEvent() {
    if (Serial.available() > 0) {
        int cnt = Serial.readBytesUntil(NULL, pcmd, BUFSIZE - (pcmd - cmd));
        pcmd += cnt;           
        if(cnt > 0) {
            done = true;
            pcmd = cmd;       
        }
    }
}

void loop() {
    if (done) {                        
        char *foo = strtok(cmd, ":");
        Serial.write("Received command:");
        Serial.write(foo);
        float dx = atof(strtok(0,":"));
        float dy = atof(strtok(0,":"));
        float dz = atof(strtok(0,";"));
        
        Serial.print(dx);
        Serial.print(":");
        Serial.print(dy);
        Serial.print(":");
        Serial.print(dz);
        
//        Serial.write(cmd);
        
        float thetax = 0, thetay = 0, thetaz = 0;
        int err = dt.CalcInverse(dx, dy, dz, thetax, thetay, thetaz);
        if(err == 0)
        {
            Serial.print(":out OK:");
            Serial.print(thetax);
            Serial.print(',');
            Serial.print(thetay);
            Serial.print(',');
            Serial.print(thetaz);
            
            ServoX.write(constrain(90-thetax, ANGLE_MIN, ANGLE_MAX));
            ServoY.write(constrain(90-thetay, ANGLE_MIN, ANGLE_MAX));
            ServoZ.write(constrain(90-thetaz, ANGLE_MIN, ANGLE_MAX));
            
        } else {
            Serial.print(":out err:");
        }
                
        Serial.write(NULL);
        done = false;
    }  
}
