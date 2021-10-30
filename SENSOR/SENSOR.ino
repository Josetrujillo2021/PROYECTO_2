//Universidad Del Valle de Guatemala
//BE3015: Electrónica Digital 2
//José Trujillo
//Proyecto #2
//------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//Librerías
//----------------------------------------------------------------------------------------------------------------------
#include <Arduino.h>

//----------------------------------------------------------------------------------------------------------------------
//Definición de pines
//----------------------------------------------------------------------------------------------------------------------
const int Trigger = 12;   //Pin digital 2 para el Trigger del sensor
const int Echo = 13;   //Pin digital 3 para el Echo del sensor

//----------------------------------------------------------------------------------------------------------------------
//Prototipos de funciones
//----------------------------------------------------------------------------------------------------------------------
void sensorProximidad(void);
//---------------------------------------------------------------------------------------------------------------------
//Variables Globales
//----------------------------------------------------------------------------------------------------------------------

//Son de tipo long debido a que su dato es en la medición de tiempo
long t; //timepo que demora en llegar el eco
long d; //distancia en centimetros

//DELAY 1 = 10 microsegundos
long LastTime1;
long tiempo1 = 10;  

//DELAY 2 = 1000 milisegundos
long LastTime2; 
long tiempo2 = 1000; 
//----------------------------------------------------------------------------------------------------------------------
//ISR  (interrupciones)
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//CONFIGURACIÓN
//----------------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);//iniciailzamos la comunicación
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0

  //tiempos
  LastTime1=micros(); 
  LastTime2=millis();
}


//---------------------------------------------------------------------------------------------------------------------
//Loop principal
//---------------------------------------------------------------------------------------------------------------------
void loop()
{
  if(millis()-LastTime2 >= tiempo2){
    sensorProximidad();   
    LastTime2 = millis(); 
  }
}

//---------------------------------------------------------------------------------------------------------------------
//Sensor proximidad
//---------------------------------------------------------------------------------------------------------------------
void sensorProximidad(void){
  // este if permite mandar la señal de disparo del sensor con un ancho de banda de 10us en alto, luego se apaga
  if (micros()- LastTime1 >= tiempo1){
    digitalWrite(Trigger, HIGH);  
    LastTime1 =micros(); 
  }
  
  digitalWrite(Trigger, LOW);

  //obtenemos el ancho del pulso 
  t = pulseIn(Echo, HIGH);
  //escalamos el tiempo a una distancia en cm 
  d = t/59;             

  //Enviamos serialmente el valor de la distancia
  Serial.print("Distancia: ");
  Serial.print(d);      
  Serial.print("cm");
  Serial.println();
}
