#include <Arduino.h>

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
        digitalWrite(LED_BUILTIN, HIGH); // Encender el LED
        delay(1000);  // Esperar un segundo
        digitalWrite(LED_BUILTIN, LOW);  // Apagar el LED
    }
    // Al final de la cuenta regresiva, encender el LED durante 3 segundos
    digitalWrite(LED_BUILTIN, HIGH); // Encender el LED
    delay(3000);  // Esperar tres segundos
    digitalWrite(LED_BUILTIN, LOW);  // Apagar el LED
    Serial.println("¡El mundo ha sido destruido!");
}

void setup() {
    Serial.begin(9600);
    Serial.println("Inicio del programa");
    pinMode(LED_BUILTIN, OUTPUT); // Configurar el pin del LED incorporado como salida
}

void loop() {
    // Verificar la secuencia ingresada por el usuario
    char secuencia[6];
    Serial.readBytes(secuencia, 6);

    if (secuencia[0] == 'C' && verificar_clave(secuencia)) {
        salvar_mundo();
    } else {
        cuenta_regresiva();
    }
} 
