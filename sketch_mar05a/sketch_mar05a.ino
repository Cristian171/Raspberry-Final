#include <SoftwareSerial.h>

#define PIN_SUBIR 2 // Pin para el botón de subir tiempo
#define PIN_BAJAR 3 // Pin para el botón de bajar tiempo
#define PIN_CAMARA 4 // Pin para controlar la cámara

SoftwareSerial serial(10, 11); // RX, TX

int tiempoApertura = 5; // Tiempo de apertura por defecto en segundos
boolean configurado = false;

enum Estado { CONFIGURACION, CUENTA_REGRESIVA, RADIACTIVO };
Estado estadoActual = CONFIGURACION;

void setup() {
  pinMode(PIN_SUBIR, INPUT_PULLUP);
  pinMode(PIN_BAJAR, INPUT_PULLUP);
  pinMode(PIN_CAMARA, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  switch (estadoActual) {
    case CONFIGURACION:
      if (!configurado) {
        configurarTiempoApertura();
      }
      break;
    case CUENTA_REGRESIVA:
      if (configurado) {
        contarTiempo();
      }
      break;
    case RADIACTIVO:
      emitirRadiacion();
      break;
  }
}


void configurarTiempoApertura() {
  Serial.println("CONFIG");
  while (!configurado) {
    if (Serial.available() > 0) {
      char input = Serial.read();
      if (input == 'S') {
        tiempoApertura++;
        if (tiempoApertura > 40) tiempoApertura = 40;
      } else if (input == 'B') {
        tiempoApertura--;
        if (tiempoApertura < 1) tiempoApertura = 1;
      } else if (input == 'L') {
        configurado = true;
        Serial.print("Tiempo configurado: ");
        Serial.println(tiempoApertura);
        estadoActual = CUENTA_REGRESIVA;
      }
    }
  }
}

void contarTiempo() {
  boolean conteoActivo = true; // Bandera para indicar si el conteo está activo
  for (int i = tiempoApertura - 1; i >= 0 && conteoActivo; i--) {
    Serial.print(i); // Imprime el número
    Serial.print(" "); // Agrega un espacio

    // Verifica si se ingresó la clave para detener el conteo
    if (Serial.available() > 0) {
      char input = Serial.read();
      if (input == 'C') {
        // Espera por los siguientes caracteres de la clave
        while (Serial.available() < 4) {}
        char clave[5]; // Almacena la clave ingresada
        for (int j = 0; j < 4; j++) {
          clave[j] = Serial.read();
        }
        clave[4] = '\0'; // Terminador de cadena
        if (strcmp(clave, "1234") == 0) {
          conteoActivo = false; // Desactiva el conteo si la clave es correcta
          Serial.println("Clave correcta. Deteniendo el conteo.");
          return; // Salir de la función contarTiempo() después de detener el conteo
        }
      }
    }

    delay(1000);
  }
  
  Serial.println(0); // Imprime el último segundo (0) sin salto de línea
  delay(1000); // Espera un segundo adicional antes de cambiar de estado
  Serial.println(); // Imprime un salto de línea para separar las salidas
  
  estadoActual = conteoActivo ? RADIACTIVO : CONFIGURACION; // Cambia el estado dependiendo si el conteo está activo o no
}


void emitirRadiacion() {
  digitalWrite(PIN_CAMARA, HIGH);
  Serial.println("RADIACIÓN NUCLEAR ACTIVA");
  delay(2000);
  digitalWrite(PIN_CAMARA, LOW);
  configurado = false;
  estadoActual = CONFIGURACION;
}
