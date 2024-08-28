#include <SoftwareSerial.h>

// Configura la comunicación serial en los pines 10 (RX) y 11 (TX)
SoftwareSerial serial(10, 11); 

// Variables globales
int tiempoApertura = 5; // Tiempo de apertura por defecto en segundos
boolean configurado = false; // Bandera que indica si el tiempo de apertura ya ha sido configurado

// Definición de los posibles estados del sistema
enum Estado { CONFIGURACION, CUENTA_REGRESIVA, RADIACTIVO };
Estado estadoActual = CONFIGURACION; // Inicializa el estado actual en "CONFIGURACION"

// Declaración de funciones
void configurarTiempoApertura();
void contarTiempo();
void emitirRadiacion();

void setup() {
  // Inicia la comunicación serial a 9600 baudios
  Serial.begin(19200);
}

void loop() {
  // Bucle principal del programa que evalúa el estado actual y llama a la función correspondiente
  switch (estadoActual) {
    case CONFIGURACION:
      if (!configurado) {
        // Si no está configurado, se llama a la función para configurar el tiempo de apertura
        configurarTiempoApertura();
      }
      break;
    case CUENTA_REGRESIVA:
      if (configurado) {
        // Si ya está configurado, se inicia la cuenta regresiva
        contarTiempo();
      }
      break;
    case RADIACTIVO:
      // Si se ha alcanzado este estado, se emite "radiación"
      emitirRadiacion();
      break;
  }
}

void configurarTiempoApertura() {
  // Informa al usuario que está en modo de configuración
  Serial.println("CONFIG");
  while (!configurado) {
    // Espera la entrada de datos desde el puerto serial
    if (Serial.available() > 0) {
      char input = Serial.read(); // Lee el carácter ingresado
      if (input == 'S') {
        // Si se presiona 'S', incrementa el tiempo de apertura
        tiempoApertura++;
        if (tiempoApertura > 40) tiempoApertura = 40; // Limita el tiempo máximo a 40 segundos
        Serial.print(" Tiempo actual: "); // Muestra el tiempo actual
        Serial.println(tiempoApertura);
      } else if (input == 'B') {
        // Si se presiona 'B', decrementa el tiempo de apertura
        tiempoApertura--;
        if (tiempoApertura < 1) tiempoApertura = 1; // Limita el tiempo mínimo a 1 segundo
        Serial.print(" Tiempo actual: "); // Muestra el tiempo actual
        Serial.println(tiempoApertura);
      } else if (input == 'L') {
        // Si se presiona 'L', se finaliza la configuración
        configurado = true; // Marca la configuración como completa
        Serial.print(" Tiempo configurado: "); // Muestra el tiempo configurado
        Serial.println(tiempoApertura);
        estadoActual = CUENTA_REGRESIVA; // Cambia el estado a "CUENTA_REGRESIVA"
      }
    }
  }
}

void contarTiempo() {
  boolean conteoActivo = true; // Bandera que indica si la cuenta regresiva está activa
  for (int i = tiempoApertura - 1; i >= 0 && conteoActivo; i--) {
    Serial.print(i); // Imprime el número actual de la cuenta regresiva
    Serial.print(" "); // Imprime un espacio

    // Verifica si se ingresó la clave para detener el conteo
    if (Serial.available() > 0) {
      char input = Serial.read(); // Lee el carácter ingresado
      if (input == 'C') {
        delay(100);  // Breve pausa para esperar la entrada de los caracteres
        if (Serial.available() >= 4) {
          char clave[6];  // Arreglo para almacenar la clave completa, incluyendo la 'C'
          clave[0] = input;  // Almacena la 'C' como primer carácter
          for (int j = 1; j < 5; j++) {
            clave[j] = Serial.read(); // Lee los siguientes 4 caracteres
          }
          clave[5] = '\0';  // Añade el terminador de cadena para poder comparar

          // Verifica si la clave es exactamente "C1234" y no hay caracteres adicionales
          if (strcmp(clave, "C1234") == 0 && Serial.available() == 0) {
            conteoActivo = false; // Detiene el conteo si la clave es correcta
            Serial.println(" Clave correcta. Deteniendo el conteo.");
            estadoActual = CONFIGURACION; // Cambia el estado a "CONFIGURACION"
            configurado = false; // Resetea la configuración
            return; // Sale de la función para detener el conteo
          } else {
            Serial.println(" Clave incorrecta o demasiados caracteres. Continuando el conteo.");
          }
        } else {
          Serial.println(" Clave incompleta. Continuando el conteo.");
        }
      }
    }

    delay(1000); // Espera un segundo antes de continuar la cuenta regresiva
  }

  // Solo imprime el 0 si se llega al final del conteo y no se ha detenido
  if (conteoActivo) {
    Serial.println(0); // Imprime el último número de la cuenta regresiva
    delay(1000); // Espera un segundo antes de cambiar de estado
    Serial.println(); // Imprime un salto de línea para separar las salidas
    estadoActual = RADIACTIVO; // Cambia el estado a "RADIACTIVO"
  }
}

void emitirRadiacion() {
  // Simula la emisión de "radiación nuclear"
  Serial.println(" RADIACIÓN NUCLEAR ACTIVA");
  delay(2000); // Espera 2 segundos para simular la radiación
  configurado = false; // Resetea la configuración
  estadoActual = CONFIGURACION; // Cambia el estado a "CONFIGURACION"
}
