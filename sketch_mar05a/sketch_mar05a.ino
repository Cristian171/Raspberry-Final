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
        Serial.print(" Tiempo actual: "); // Agrega un espacio antes de mostrar el tiempo
        Serial.println(tiempoApertura);
      } else if (input == 'B') {
        tiempoApertura--;
        if (tiempoApertura < 1) tiempoApertura = 1;
        Serial.print(" Tiempo actual: "); // Agrega un espacio antes de mostrar el tiempo
        Serial.println(tiempoApertura);
      } else if (input == 'L') {
        configurado = true;
        Serial.print(" Tiempo configurado: "); // Agrega un espacio antes de mostrar el tiempo
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
        delay(100);  // Pausa para asegurar que los caracteres estén disponibles
        if (Serial.available() >= 4) { // Asegura que hay 4 caracteres disponibles
          char clave[5]; // Almacena la clave ingresada
          for (int j = 0; j < 4; j++) {
            clave[j] = Serial.read();
          }
          clave[4] = '\0'; // Terminador de cadena
          if (strcmp(clave, "1234") == 0) {
            conteoActivo = false; // Desactiva el conteo si la clave es correcta
            Serial.println(" Clave correcta. Deteniendo el conteo."); // Espacio antes del mensaje
            estadoActual = CONFIGURACION;
            configurado = false;
            return; // Salir de la función contarTiempo() después de detener el conteo
          } else {
            Serial.println(" Clave incorrecta. Continuando el conteo."); // Espacio antes del mensaje
          }
        } else {
          Serial.println(" Clave incompleta. Continuando el conteo."); // Espacio antes del mensaje
        }
      }
    }

    delay(1000);
  }

  // Eliminar el segundo 0 extra
  if (conteoActivo) {
    Serial.println(0); // Solo imprime 0 una vez
    delay(1000); // Espera un segundo adicional antes de cambiar de estado
    Serial.println(); // Imprime un salto de línea para separar las salidas
  }
  
  estadoActual = RADIACTIVO; // Cambia el estado después del conteo
}

void emitirRadiacion() {
  Serial.println(" RADIACIÓN NUCLEAR ACTIVA"); // Espacio antes del mensaje
  delay(2000);
  configurado = false;
  estadoActual = CONFIGURACION;
}
