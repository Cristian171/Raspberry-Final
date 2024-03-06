#include <Arduino.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define CLAVE_CORRECTA "1234"

const int ledPin = PICO_DEFAULT_LED_PIN; // Definir el pin del LED integrado

bool verificar_clave(char *secuencia) {
    char clave[6];
    sprintf(clave, "C%s", CLAVE_CORRECTA);
    return strcmp(secuencia, clave) == 0;
}

void salvar_mundo() {
    Serial.println("¡SALVASTE AL MUNDO!");
    
    // Encender el LED
    gpio_init(ledPin);
    gpio_set_dir(ledPin, GPIO_OUT);
    gpio_put(ledPin, 1); // 1 enciende el LED
}

void cuenta_regresiva() {
    int tiempo = 10;
    while (tiempo > 0) {
        Serial.println(tiempo);
        delay(1000);  // Esperar un segundo
        tiempo--;
    }
    Serial.println("¡El mundo ha sido destruido!");

    // Apagar el LED
    gpio_put(ledPin, 0); // 0 apaga el LED
}

void setup() {
    Serial.begin(9600);
    Serial.println("Inicio del programa");
}

void loop() {
    char secuencia[6];
    Serial.readBytes(secuencia, 6);
    
    if (secuencia[0] == 'C' && verificar_clave(secuencia)) {
        salvar_mundo();
    } else {
        cuenta_regresiva();
    }
}
