#include <Servo.h>

Servo leftright;
Servo updown;
int topleft;
int topright;
int downleft;
int downright;
int horizontal = 90;
int vertical = 100;
int waittime = 30;


void setup() {
  Serial.begin(9600);
  leftright.attach(9);
  updown.attach(10);
  leftright.write(horizontal);
  updown.write(vertical);
}


void moveright() {
  horizontal++;
  leftright.write(horizontal);
}

void moveleft() {
  horizontal--;
  leftright.write(horizontal);
}

void movedown() {
  vertical--;
  updown.write(vertical);
}

void moveup() {
  vertical++;
  updown.write(vertical);
}

void loop() {
  topleft = analogRead(A2);
  topright = analogRead(A1);
  downleft = analogRead(A3);
  downright = analogRead(A0);

  if (horizontal > 180)
    horizontal = 180;
  if (horizontal < 0)
    horizontal = 0;
  if (vertical > 150)
    vertical = 150;
  if (vertical < 60)
    vertical = 60;

  if ((downright < topright) or (downleft < topleft)) {
    movedown();
    delay(waittime);
  }

  if ((downright > topright) or (downleft > topleft)) {
    moveup();
    delay(waittime);
  }

  if ((downright < downleft) or (topright < topleft)) {
    moveright();
    delay(waittime);
  }

  if ((topright > topleft) or (downright > downleft)) {
    moveleft();
    delay(waittime);
  }
}
