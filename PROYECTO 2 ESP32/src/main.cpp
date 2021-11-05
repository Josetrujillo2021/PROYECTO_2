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


#define Trigger 32   //Pin digital 2 para el Trigger del sensor
#define Echo 14   //Pin digital 3 para el Echo del sensor

//----------------------------------------------------------------------------------------------------------------------
//Prototipos de funciones
//----------------------------------------------------------------------------------------------------------------------
void sensorProximidad(void);
void guardarDatoSD(void);
//---------------------------------------------------------------------------------------------------------------------
//Variables Globales
//----------------------------------------------------------------------------------------------------------------------

//Son de tipo long debido a que su dato es en la medición de tiempo
long t; //timepo que demora en llegar el eco
long d; //distancia en centimetros

//DELAY 1 = 10 microsegundos
long LastTime1;

//DELAY 2 = 1000 milisegundos
long LastTime2; 
long tiempo2 = 1000; 

String medidor = ""; 
String guardado= ""; 
//----------------------------------------------------------------------------------------------------------------------
//ISR  (interrupciones)
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//CONFIGURACIÓN
//----------------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);//iniciailzamos la comunicación
  Serial2.begin(115200);
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
  sensorProximidad();   
  guardarDatoSD(); 
}

//---------------------------------------------------------------------------------------------------------------------
//Sensor proximidad
//---------------------------------------------------------------------------------------------------------------------
void sensorProximidad(void){
  // este while permite mandar la señal de disparo del sensor con un ancho de banda de 10us en alto, luego se apaga
  digitalWrite(Trigger, HIGH); 
  LastTime1=micros();  
  while(micros()<LastTime1+10) ; 
  digitalWrite(Trigger, LOW);

  //obtenemos el ancho del pulso 
  t = pulseIn(Echo, HIGH);
  //escalamos el tiempo a una distancia en cm 
  d = t/59;             

  //permite leer lo que se mande al canal serial 1 hasta que exista un espacio y lo guarda en la variable 
  medidor = Serial2.readStringUntil('\n');

  //Enviamos serialmente el valor de la distancia
  if (medidor == "medir"){
      //Comunicación con monitor
      Serial.print("Distancia: ");
      Serial.print(d);      
      Serial.print("cm");
      Serial.println();  
      //Comunicación con TIVA
      Serial2.println(d);
      medidor = ""; 
  } 
}

//---------------------------------------------------------------------------------------------------------------------
//Guardar datos en la SD
//---------------------------------------------------------------------------------------------------------------------
void guardarDatoSD(void){
  //permite leer lo que se mande al canal serial 1 hasta que exista un espacio y lo guarda en la variable 
  guardado= Serial2.readStringUntil('\n');
  if (guardado == "guardar"){
    //Comunicacion con UART
    Serial.println("Se guardaron los datos en la memoria SD"); 
    //Se envia el dato a TIVA para que lo guarde
    Serial2.println(d);
    guardado = ""; 
  }
}