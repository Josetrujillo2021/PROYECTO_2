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

#define Boton1 PF_0 
#define Boton2 PF_4

#define Trigger PE_4   //Pin digital 2 para el Trigger del sensor
#define Echo PD_2   //Pin digital 3 para el Echo del sensor

#define buzzer PF_2

//notas 
#define NOTE_F7  2794
#define NOTE_F6  1397
#define NOTE_B0  31
#define NOTE_A7  3520
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

//Melodias del buzzer
int melody[] = { NOTE_F7, NOTE_F6 };
int medicion[] = { NOTE_B0, NOTE_A7 };

//Duración de las notas
int noteDurations[] = {4, 16};
int noteDurations1[] = {8, 8};

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

  pinMode(Boton1, INPUT_PULLUP);
  pinMode(Boton2, INPUT_PULLUP);

  
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

  //Enviamos serialmente el valor de la distancia
  if (digitalRead(Boton1)==0){
    //Alarma Buzzer
    for (int thisNote = 0; thisNote < 2; thisNote++) {
      //Tiempo de la nota
      int noteDuration1 = 1000 / noteDurations1[thisNote];
      tone(buzzer, medicion[thisNote], noteDuration1);
      //Tiempo entre nota y nota
      int pauseBetweenNotes = noteDuration1 * 1.30;
      delay(pauseBetweenNotes);
      //detiene la nota que esta sonando
      noTone(buzzer);
    }
    delay(10);
    if(digitalRead(Boton1)==1){
      Serial.print("Distancia: ");
      Serial.print(d);      
      Serial.print("cm");
      Serial.println();  
   }
  } 
}

//---------------------------------------------------------------------------------------------------------------------
//Guardar datos en la SD
//---------------------------------------------------------------------------------------------------------------------
void guardarDatoSD(void){
 if (digitalRead(Boton2)==0){
   for (int thisNote = 0; thisNote < 2; thisNote++) {
      //Alarma Buzzer
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(buzzer, melody[thisNote], noteDuration);
      //iempo entre nota y nota
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      //se detiene la nota que esta sonando
      noTone(buzzer);
    }
   delay(10);
    if(digitalRead(Boton2)==1){
      //aqui debo de agragar la función que se comunicara con ESP32 para guardar el dato
      Serial.println("Se guardaron los datos en la memoria SD"); 
    }
}
}
