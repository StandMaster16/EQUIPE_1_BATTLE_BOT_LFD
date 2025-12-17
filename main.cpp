#include "BluetoothSerial.h"
#include <ESP32Servo.h>

//Pinos motores

// Motor Esquerda
#define IN1 27
#define IN2 26
#define ENA 14

// Motor Direita
#define IN3 25
#define IN4 33
#define ENB 32

// Pino do Servo
#define SERVO 23

// Pino do sensor
#define TRIG 4
#define ECHO 16
#define c 0.0343

#define BLUE1 5
#define BLUE2 21

#define RED1 18
#define RED2 19

BluetoothSerial SerialBT;
Servo servo;

int velocity = 200;
String message = ""; 
String currentState = "parar";
int pos = 45; // Servo
unsigned long movTime = 0;
const long movInterval = 2000;
const long movInterval2 = 500;

float pulse, distance;
unsigned long distanceTime = 0;
const long distanceInterval = 500;

void move(String msg){

      if (msg == "up" || msg == "dash"){
        digitalWrite(IN1, HIGH); 
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH); 
        digitalWrite(IN4, LOW);
        analogWrite(ENA, velocity); 
        analogWrite(ENB, velocity);
        Serial.println("Eu estive aqui");

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

  pinMode(RED1, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(BLUE1, OUTPUT);
  pinMode(BLUE2, OUTPUT);

  digitalWrite(RED1, LOW);
  digitalWrite(RED2, LOW);
  digitalWrite(BLUE1, HIGH);
  digitalWrite(BLUE2, HIGH);

}

void loop() {
  
  unsigned long currentMillis = millis();

  if (SerialBT.available()){

    message = SerialBT.readString();
    message.trim();

    if (!message.isEmpty()){
      Serial.println("Mensagem Recebida do App: [" + message + "]");
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

  } else if (currentState == "left" || currentState == "right"){

    if (currentMillis-movTime < movInterval2) move (currentState);
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
      digitalWrite(RED1, HIGH);
      digitalWrite(RED2, HIGH);
      digitalWrite(BLUE1, LOW);
      digitalWrite(BLUE2, LOW);
  
    } else {
      servo.write(0);
      digitalWrite(RED1, LOW);
      digitalWrite(RED2, LOW);
      digitalWrite(BLUE1, HIGH);
      digitalWrite(BLUE2, HIGH);
    }

  }

}