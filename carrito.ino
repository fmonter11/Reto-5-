//Definimos los pines a donde estan conectados los los motores y sus enables
#define MotA 10
#define MotB 11
#define Der1 8
#define Der0 9
#define Izq1 13
#define Izq0 12

//Variable que nos indica el valor de la luz ambiente
int Ambiente = 500;

//Infrarrojo
#define IRD A0
#define IRI A1
int distancia2;
int distancia3;

//LDR
#define LDR1 A2
#define LDR2 A5
#define LDR3 A3
#define LDR4 A4
//Variables auxiliares para guardar las lecturas de los sensores 
//y sus comparaciones
int val1, val2, val3, val4;
int vm, comp1, comp2, comp3, comp4;


//Ultrasonico
#define trig 53    // Disparador (Trigger)
#define echo 52
long duracion, distancia;

void setup() {
  //Inicializamos el monitor serial
  Serial.begin(9600);
  //Declaramos nuestros pines como entradas o salidas 
  pinMode(MotA, OUTPUT);
  pinMode(MotB, OUTPUT);
  pinMode(Der1, OUTPUT);
  pinMode(Der0, OUTPUT);
  pinMode(Izq1, OUTPUT);
  pinMode(Izq0, OUTPUT);
  //Mandamos el valor del vaoltaje de 0 a 255 que hayamos calibrado en la practica
  analogWrite(MotA, 255);
  analogWrite(MotB, 183);
  //LDR
  pinMode(LDR1, INPUT);
  pinMode(LDR2, INPUT);
  pinMode(LDR3, INPUT);
  pinMode(LDR4, INPUT);
  //Ultrasonico
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

}

void loop() {
  //Metodo para obtener las mediciones de los sensores LDR
  luzMayor();
  //Condicional en valor maximo del LDR para frenar el robot cuando llegue a la meta
  if(vm < 980){
    SeguirLuz();
  }else{
    Stop();
  }
  //Metodos comentados para hacer pruebas 
  //SeguirLuz();
  //Ultrasonico();
  //Evade();
  //IR_sensor();
  //delay(1000);
}

//Metodo de evasion de obstaculos utilizando 1 sensor ultrasonico y 2 sensores Sharp
void Evade(){
  IR_sensor();
  Ultrasonico();
  int diferencia = distancia2 - distancia3;
  if(distancia3 < 6 ){
    Right();
    }else if (distancia2 < 6){
      Left();
      }else if((diferencia == 0 && distancia2 < 20) or distancia < 10 or distancia == 0 or distancia2 == 0 or distancia3==0 ){
        Backward();
        Right();
      }else{     
      Forward();
  }
}

//Metodo de evasion de obstaculos version 2, para hacer pruebas
void Evade1(){
  IR_sensor();
  int diferencia = distancia2 - distancia3;
  if(distancia3 < 5 ){
    Right();
    }else if (distancia2 < 5){
      Left();
      }else if(diferencia == 0 && distancia2 < 25 ){
        Backward();
      }else{     
      Forward();
  }
}


// Metodo para seguir la luz
//Basado en los valores maximos obtenidos de los sensores LDR
void SeguirLuz(){
  if( vm > Ambiente){
    if(vm == val1 or vm==val2){
      Evade();
      //Forward();
      }else if(vm == val3){
        Evade();
        Right();
        }else if (vm == val4){
          Evade();
          Left();
        }else{
          Stop();
        }
  delay(5);
  }else {
    Stop();
  }
}

//Metodo que obtiene el primedio de 6 mediciones en cada uno de los 4 sensores LDR
//Obtiene el valor maximo e imprime los valores en el monitor serial
void luzMayor(){
  for(int i=0;i<5;i++){
    int luz1 = analogRead(LDR1);
    val1 += luz1;
  }
  val1 /=6; //Promedio del sensor 1

  for(int i=0;i<5;i++){
    int luz2 = analogRead(LDR2);
    val2 += luz2;
  }
  val2 /=6; //Promedio del sensor 2

  for(int i=0;i<5;i++){
    int luz3 = analogRead(LDR3);
    val3 += luz3;
  }
  val3 /=6; //Promedio del sensor 3

  for(int i=0;i<5;i++){
    int luz4 = analogRead(LDR4);
    val4 += luz4;
  }
  val4 /=6; //Promedio del sensor 4

  //Valor maximo
  comp1 = max(val1, val2); 
  comp2 = max(val3,val4);
  comp3 = max(comp1, val3);
  comp4 = max(comp2, val2);
  vm = max(comp1, comp2);

  Serial.print("Sensor 1: ");Serial.println(val1);
  Serial.print("Sensor 2: ");Serial.println(val2);
  Serial.print("Sensor 3: ");Serial.println(val3);
  Serial.print("Sensor 4: ");Serial.println(val4);
  Serial.print("Valor Maximo: ");Serial.println(vm);
  Serial.println("---------------");
  Serial.println("  ");

}


//Lectura de los sensores infrarojos
void IR_sensor(){
  int lectura = analogRead(IRD);
  int lectura2 = analogRead(IRI);
  distancia2 = 2076/(lectura-11);
  distancia3 = 2076/(lectura2-11);
  Serial.print("Infrarrojo derecho: ");Serial.print(distancia2); Serial.println(" cm");
  Serial.print("Infrarrojo izquierdo: ");Serial.print(distancia3);Serial.println(" cm");
}

//Lectura del Sensor Ultrasonico 
void Ultrasonico(){
  digitalWrite(trig, HIGH);
  delay(1);
  digitalWrite(trig, LOW);
  duracion = pulseIn(echo, HIGH);
  //Valor especificado por el fabricante
  distancia = duracion/58.2;
  Serial.print("Distancia: "); Serial.print(distancia);Serial.println(" cm "); 
}

//Metodo para hacer la prueba de movimiento en L
//Los tiempos fueron calibrados experimentalmente
void Rutina(){
  Stop();
  delay(1000);
  Forward();
  delay(6600);
  Right();
  delay(250);
  Forward();
  delay(3000);
  Stop();
  delay(5000);
}

//Metodos basicos de avance 
//Lo unico que hicimos fue definir la izquierda y la derecha y si sus respectivos enables

//Metodo para avanzar hacia adelante
void Forward() {
  analogWrite(MotA, 255);
  analogWrite(MotB, 183);
  digitalWrite(Der0, LOW);
  digitalWrite(Der1, HIGH);
  digitalWrite(Izq0, LOW);
  digitalWrite(Izq1, HIGH); 
  Serial.println("Forward");
}
//Metodo para avanzar hacia atras
void Backward() {
  digitalWrite(Der0, HIGH);
  digitalWrite(Der1, LOW);
  digitalWrite(Izq0, HIGH);
  digitalWrite(Izq1, LOW); 
  Serial.println("Backward");
}

//Metodo para avanzar a la derecha
void Right() {
  analogWrite(MotA, 150);
  analogWrite(MotB, 100);
  digitalWrite(Der0, HIGH);
  digitalWrite(Der1, LOW);
  digitalWrite(Izq0, LOW);
  digitalWrite(Izq1, HIGH); 
  Serial.println("Right");
  
}

//Metodo para avanzar a la izquierda
void Left() {
  analogWrite(MotA, 150);
  analogWrite(MotB, 100);
  digitalWrite(Der0, LOW);
  digitalWrite(Der1, HIGH);
  digitalWrite(Izq0, HIGH);
  digitalWrite(Izq1, LOW); 
  Serial.println("Left");
  
}

//Metodo para no avanzar
void Stop() {
  digitalWrite(Der0, LOW);
  digitalWrite(Der1, LOW);
  digitalWrite(Izq0, LOW);
  digitalWrite(Izq1, LOW); 
  Serial.println("Stop");
}
