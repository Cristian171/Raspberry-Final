#include <Arduino.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define CLAVE_CORRECTA "1234"

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
    for (int tiempo = 10; tiempo > 0; tiempo--) {
        Serial.println(tiempo);
        delay(1000);  // Esperar un segundo
    }
    Serial.println("¡El mundo ha sido destruido!");
}

void setup() {
    Serial.begin(9600);
    Serial.println("Inicio del programa");
    pinMode(LED_BUILTIN, OUTPUT); // Configura el pin del LED incorporado como salida
}

void loop() {
    // Parpadeo del LED
    static uint32_t previousTime = 0;
    static bool ledState = true;

    uint32_t currentTime = millis();

    if( (currentTime - previousTime) > 100){
        previousTime = currentTime;
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
    }

    // Verificar la secuencia ingresada por el usuario
    char secuencia[6];
    Serial.readBytes(secuencia, 6);

    if (secuencia[0] == 'C' && verificar_clave(secuencia)) {
        salvar_mundo();
    } else {
        cuenta_regresiva();
    }
}
