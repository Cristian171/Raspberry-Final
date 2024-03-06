#include <Arduino.h>

#define CLAVE_CORRECTA "1234"

int tiempoInicial = 10;  // Tiempo inicial del temporizador en segundos
unsigned long previousMillis = 0;  // Para el control del tiempo
bool cuentaRegresivaActiva = false;  // Para controlar si la cuenta regresiva está activa

bool verificar_clave(char *secuencia) {
    char clave[6];
    sprintf(clave, "C%s", CLAVE_CORRECTA);
    return strcmp(secuencia, clave) == 0;
}

void salvar_mundo() {
    Serial.println("¡SALVASTE AL MUNDO!");
}

void cuenta_regresiva() {
    Serial.println("Comienza la cuenta regresiva:");
    cuentaRegresivaActiva = true;
    for (int tiempo = tiempoInicial; tiempo > 0; tiempo--) {
        Serial.println(tiempo);
        delay(1000);  // Esperar un segundo
    }
    cuentaRegresivaActiva = false;
    Serial.println("¡El mundo ha sido destruido!");
}

void setup() {
    Serial.begin(9600);
    Serial.println("Inicio del programa");
}

void loop() {
    char input;

    // Leer la entrada serial si está disponible
    if (Serial.available() > 0) {
        input = Serial.read();

        // Verificar la clave si se recibe un comando 'C'
        if (input == 'C') {
            char secuencia[6];
            Serial.readBytesUntil('\n', secuencia, 6);
            if (verificar_clave(secuencia)) {
                salvar_mundo();
            } else {
                cuenta_regresiva();
            }
        }

        // Comenzar la cuenta regresiva si se recibe un comando 'S'
        if (input == 'S') {
            cuenta_regresiva();
        }
    }

    // Si la cuenta regresiva está activa, no hacer nada más en el bucle
    if (cuentaRegresivaActiva) {
        return;
    }

}



