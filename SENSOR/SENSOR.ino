#include <Arduino.h>
#define Trigger PE_4
#define Echo PD_2
//const int Trigger = 12;   //Pin digital 2 para el Trigger del sensor
//const int Echo = 13;   //Pin digital 3 para el Echo del sensor
long InicialTime; 
void setup() {
  Serial.begin(115200);//iniciailzamos la comunicación
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
  InicialTime=micros(); 
}

void loop()
{
  long t; //timepo que demora en llegar el eco
  long d; //distancia en centimetros
  long t2; 
  long d2;
  
  int V1 = 0; 
  
  
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);

  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso //preguntar si puedo usar transistores para poder comunicar al SENSOR con el OPAM. el triger va conectado a 5V y el transitor  del pin detrigger del 
  //procesador va a 3.3V luego la del transistor del echo del sensor ba a la base y 3.3v a el procesador
  d = t/59;             //escalamos el tiempo a una distancia en cm

  
  Serial.print("Distancia: ");
  Serial.print(d);      //Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println();
  Serial.println(t);
  
  delay(1000);      //Hacemos una pausa de 100ms
}
