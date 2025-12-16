#include "BluetoothSerial.h"
#include <ESP32Servo.h>

//Pinos motores

// Motor Esquerda
#define IN1 13
#define IN2 14
#define ENA 15

// Motor Direita
#define IN3 21
#define IN4 22
#define ENB 23

// Pino do Servo
#define SERVO 25

// Pino do sensor
#define TRIG 26
#define ECHO 27
#define c 0.0343

#define RED 33
#define RGB 28

BluetoothSerial SerialBT;
Servo servo;

int velocity = 255;
String message = ""; 
String currentState = "parar";
int pos = 45; // Servo
unsigned long movTime = 0;
const long movInterval = 2000;

float pulse, distance;
unsigned long distanceTime = 0;
const long distanceInterval = 500;

void move(String msg){

      if (msg == "up" || msg == "dash"){
        digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
        analogWrite(ENA, velocity); analogWrite(ENB, velocity);

      } else if (msg == "down" || msg == "recuo"){
        digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
        analogWrite(ENA, velocity); analogWrite(ENB, velocity);

      } else if (msg == "left"){
        digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
        analogWrite(ENA, velocity); analogWrite(ENB, velocity);

      } else if (msg == "right"){
        digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
        analogWrite(ENA, velocity); analogWrite(ENB, velocity);
      
      } else if (msg == "parar"){
        digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
        analogWrite(ENA, 0); analogWrite(ENB, 0);
      }

}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_EQUIPE1");
  Serial.println("Dispositivo iniciado");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  servo.attach(SERVO);
  servo.write(0);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(RED, OUTPUT);

}

void loop() {
  
  unsigned long currentMillis = millis();

  if (SerialBT.available()){

    message = SerialBT.readString();
    message.trim();

    if (!message.isEmpty()){
      if (message == "up" || message == "down" || message == "left" || message == "right" || message == "parar"){
        currentState = message;
        movTime = 0;
      
      } else if (message == "dash" || message == "recuo"){
        currentState = message;
        movTime = currentMillis;
      }
    }

  }

  if (currentState == "dash" || currentState == "recuo"){

    if (currentMillis-movTime < movInterval) move(currentState);
    else {
      currentState = "parar";
      move(currentState);
    }

  } else {
    move(currentState);
  }

  if (currentMillis-distanceTime >= distanceInterval) {
    distanceTime = currentMillis;

    digitalWrite(TRIG, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    pulse = pulseIn(ECHO, HIGH);
    distance = (pulse*c)/2;
    SerialBT.println(String(distance));

    if (distance <= 30 && distance > 0){
      servo.write(pos);
      digitalWrite(RED, HIGH);
  
    } else {
      servo.write(0);
      digitalWrite(RED, LOW);
    }

  }

}