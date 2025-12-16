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

#define LED1 33

BluetoothSerial SerialBT;
Servo servo;

int velocidade = 255;
String message = "";
int pos = 45;
float pulso, distancia;

unsigned long lastDistanceTime = 0;
const long distanceInterval = 200;

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

  pinMode(LED1, OUTPUT);

}

void loop() {

  if (SerialBT.available()){

    message = SerialBT.readString();
    message.trim();

  }

  if (!message.isEmpty()){

    if (message == "up"){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);

      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

      analogWrite(ENA, velocidade);
      analogWrite(ENB, velocidade);
    }

    else if (message == "down"){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

      analogWrite(ENA, velocidade);
      analogWrite(ENB, velocidade);
    }

    else if (message == "left"){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);

      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

      analogWrite(ENA, velocidade);
      analogWrite(ENB, velocidade);
    }

    else if (message == "right"){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

      analogWrite(ENA, velocidade);
      analogWrite(ENB, velocidade);
    }

    else if (message == "parar"){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);

      analogWrite(ENA, 0);
      analogWrite(ENB, 0);
    }

    else if (message == "dash"){
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);

      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

      analogWrite(ENA, velocidade);
      analogWrite(ENB, velocidade);

      delay(2000);
    }

    else if (message == "recuo"){
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);

      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

      analogWrite(ENA, velocidade);
      analogWrite(ENB, velocidade);

      delay(2000);
    }

    message = "";

  }

  unsigned long currentMillis = millis();

  digitalWrite(TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  pulso = pulseIn(ECHO, HIGH);
  distancia = (pulso*c)/2;
  SerialBT.println(distancia);

  if (distancia <= 30){
    servo.write(pos);
    digitalWrite(LED1, HIGH);
    delay(1000);
    servo.write(0);
  }

  delay(1000);

}
