//Universidad Del Valle de Guatemala
//BE3015: Electrónica Digital 2
//José Trujillo
//Proyecto #2
//------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//Librerías
//----------------------------------------------------------------------------------------------------------------------
#include <Arduino.h>

//librerías para TFT
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

//----------------------------------------------------------------------------------------------------------------------
//Definición de pines
//----------------------------------------------------------------------------------------------------------------------

#define Boton1 PF_0 
#define Boton2 PF_4


#define buzzer PF_2

//notas 
#define NOTE_F7  2794
#define NOTE_F6  1397
#define NOTE_B0  31
#define NOTE_A7  3520

//Pines para TFT 
#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};
//----------------------------------------------------------------------------------------------------------------------
//Prototipos de funciones
//----------------------------------------------------------------------------------------------------------------------
void sensorProximidad(void);
void guardarDatoSD(void);
void lecturaDatos(void);

//funciones para TFT
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset);


//---------------------------------------------------------------------------------------------------------------------
//Variables Globales
//----------------------------------------------------------------------------------------------------------------------
//Variable para TFT 
extern uint8_t fondo[];

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

//Dato sensor
String dato=""; 
//----------------------------------------------------------------------------------------------------------------------
//ISR  (interrupciones)
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//CONFIGURACIÓN
//----------------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);//iniciailzamos la comunicación
  Serial3.begin(115200);
  /*pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada*/

  pinMode(Boton1, INPUT_PULLUP);
  pinMode(Boton2, INPUT_PULLUP);

  
  //digitalWrite(Trigger, LOW);//Inicializamos el pin con 0

  //tiempos
  LastTime1=micros(); 
  LastTime2=millis();

  //Inicializacion de TFT
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

  LCD_Init();
  LCD_Clear(0x00);
  FillRect(0, 0, 319, 239, 0xFFFF);
  FillRect(50, 60, 20, 20, 0xF800);
  FillRect(70, 60, 20, 20, 0x07E0);
  FillRect(90, 60, 20, 20, 0x001F);

  String text1 = "Sensor de proximidad";
}


//---------------------------------------------------------------------------------------------------------------------
//Loop principal
//---------------------------------------------------------------------------------------------------------------------
void loop()
{
   for (int x = 0; x < 320 - 32; x++) {
    int anim2 = (x / 35) % 4;
    LCD_Sprite(60, 100, 32, 32, pesaSprite, 4, anim2, 0, 1);
    delay(15);
  }
  sensorProximidad();   
  guardarDatoSD(); 
  lecturaDatos();
}

//---------------------------------------------------------------------------------------------------------------------
//Sensor proximidad
//---------------------------------------------------------------------------------------------------------------------
void sensorProximidad(void){
 /* // este while permite mandar la señal de disparo del sensor con un ancho de banda de 10us en alto, luego se apaga
  digitalWrite(Trigger, HIGH); 
  LastTime1=micros();  
  while(micros()<LastTime1+10) ; 
  digitalWrite(Trigger, LOW);

  //obtenemos el ancho del pulso 
  t = pulseIn(Echo, HIGH);
  //escalamos el tiempo a una distancia en cm 
  d = t/59;*/             

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
      Serial3.println("medir");
      /*
      Serial.print("Distancia: ");
      Serial.print(d);      
      Serial.print("cm");
      Serial.println();  */
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
      Serial3.println("guardar");
      //Serial.println("Se guardaron los datos en la memoria SD"); 
    }
}
}
//---------------------------------------------------------------------------------------------------------------------
//Lectura de datos
//---------------------------------------------------------------------------------------------------------------------
void lecturaDatos(void){
  dato = Serial3.readStringUntil('\n');
  Serial.println(dato);
}
