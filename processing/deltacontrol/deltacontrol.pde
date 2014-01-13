/**
 * Mouse Functions. 
 * 
 * Click on the box and drag it across the screen. 
 */
 import java.awt.event.*;
 import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port
int baud = 19200;
float bx;
float by;

int boxSize = 25;
boolean overBox = false;
boolean locked = false;
float xOffset = 0.0; 
float yOffset = 0.0; 
int currentWheel = 50;
boolean eventLock = false;

int BUFSIZE = 256;
byte[] response = new byte[256];
int presponse = 0;
boolean responseReceived = true;
int timeOut = 1000;

void setup() 
{
  size(640, 480);
  bx = width/2.0;
  by = height/2.0;
  rectMode(RADIUS);  
    addMouseWheelListener(new MouseWheelListener() { 
    public void mouseWheelMoved(MouseWheelEvent mwe) { 
      mouseWheel(mwe.getWheelRotation());
  }}); 
  
  myPort = new Serial(this, "COM5", baud);
  delay(1500);
  myPort.write(0);
}

void draw() 
{ 
  background(0);
  
  // Test if the cursor is over the box 
  if (mouseX > bx-boxSize && mouseX < bx+boxSize && 
      mouseY > by-boxSize && mouseY < by+boxSize) {
    overBox = true;  
    if(!locked) { 
      stroke(255); 
      fill(153);
    } 
  } else {
    stroke(153);
    fill(153);
    overBox = false;
  }
  
  // Draw the box
  rect(bx, by, boxSize, boxSize);
  
  //map the current bx,by, and mouseWheel
  float botx = map(bx, 100,640-100,-100,100);
  float boty = map(by, 100,480-100,-100,100);
  float botz = map(currentWheel, 0,100,-128,-240);
    
  //only write commands after we receive responses
  if(responseReceived) 
  {
    presponse = 0;
    responseReceived = false;
    
    myPort.write(getCmd(botx, boty, botz));
    myPort.write(0);
  } 
  /*
  else 
  {
    timeOut--;
    if(timeOut <= 0) 
    {
      println("Timeout, retrying...");
      myPort.clear();
      presponse = 0;
      responseReceived = true;
      timeOut = 1000;
    }    
  }
  */
}

void serialEvent(Serial p) {  
    while(p.available() > 0) {            
      char data= p.readChar();
        response[presponse++] = (byte)data;        
        if(data == 0) {
            responseReceived = true;
            int len = presponse;
            print("[");
            print(new String(response, 0, len));
            print("]");
            print(":");
            print(len);
            println(":OK");
            break;
        }
    }
}

String getCmd(float x, float y, float z) {
  String command = new String();
  command += "MV:";
  command += x;
  command += ":";
  command += y;
  command += ":";
  command += z;
  command += ";";    
  return command;
}

void mousePressed() {
  if(overBox) { 
    locked = true; 
    fill(255, 255, 255);
  } else {
    locked = false;
  }
  xOffset = mouseX-bx; 
  yOffset = mouseY-by; 

}

void mouseDragged() {
  if(locked) {
    bx = constrain(mouseX-xOffset, 100, 640-100); 
    by = constrain(mouseY-yOffset, 100, 480-100); 
  }
}

void mouseReleased() {
  locked = false;
}

void mouseWheel(int delta) {
 currentWheel+=delta;
 currentWheel = constrain(currentWheel, 0, 100); 
}




