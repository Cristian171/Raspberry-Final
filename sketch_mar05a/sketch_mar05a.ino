#include <SoftwareSerial.h>

SoftwareSerial serial(10, 11); // RX, TX

int tiempoApertura = 5; // Tiempo de apertura por defecto en segundos
boolean configurado = false;

enum Estado { CONFIGURACION, CUENTA_REGRESIVA, RADIACTIVO };
Estado estadoActual = CONFIGURACION;

void configurarTiempoApertura();
void contarTiempo();
void emitirRadiacion();

void setup() {
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
        Serial.print(" Tiempo actual: ");
        Serial.println(tiempoApertura);
      } else if (input == 'B') {
        tiempoApertura--;
        if (tiempoApertura < 1) tiempoApertura = 1;
        Serial.print(" Tiempo actual: ");
        Serial.println(tiempoApertura);
      } else if (input == 'L') {
        configurado = true;
        Serial.print(" Tiempo configurado: ");
        Serial.println(tiempoApertura);
        estadoActual = CUENTA_REGRESIVA;
      }
    }
  }
}

void contarTiempo() {
  boolean conteoActivo = true;
  for (int i = tiempoApertura - 1; i >= 0 && conteoActivo; i--) {
    Serial.print(i);
    Serial.print(" ");

    // Verifica si se ingresó la clave para detener el conteo
    if (Serial.available() > 0) {
      char input = Serial.read();
      if (input == 'C') {
        delay(100);  // Breve pausa para esperar la entrada de los caracteres
        if (Serial.available() >= 4) {
          char clave[6];  // Almacena la clave completa incluyendo la 'C'
          clave[0] = input;  // 'C'
          for (int j = 1; j < 5; j++) {
            clave[j] = Serial.read();
          }
          clave[5] = '\0';  // Terminador de cadena

          // Verifica si la clave es exactamente "C1234"
          if (strcmp(clave, "C1234") == 0 && Serial.available() == 0) {
            conteoActivo = false;
            Serial.println(" Clave correcta. Deteniendo el conteo.");
            estadoActual = CONFIGURACION;
            configurado = false;
            return;
          } else {
            Serial.println(" Clave incorrecta o demasiados caracteres. Continuando el conteo.");
          }
        } else {
          Serial.println(" Clave incompleta. Continuando el conteo.");
        }
      }
    }

    delay(1000);
  }

  // Solo imprime el 0 si se llega al final del conteo y no se ha detenido
  if (conteoActivo) {
    Serial.println(0);
    delay(1000);
    Serial.println(); // Salto de línea para separar las salidas
    estadoActual = RADIACTIVO; // Cambia el estado después del conteo
  }
}

void emitirRadiacion() {
  Serial.println(" RADIACIÓN NUCLEAR ACTIVA");
  delay(2000);
  configurado = false;
  estadoActual = CONFIGURACION;
}
