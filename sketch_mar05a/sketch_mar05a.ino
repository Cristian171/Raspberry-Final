#include <Arduino.h>

#define DEBOUNCE_DELAY 50  // Retardo para la eliminación de rebotes de los botones, en milisegundos
#define CLAVE_CORRECTA "1234"

int tiempoInicial = 10;  // Tiempo inicial del temporizador en segundos
unsigned long lastDebounceTime = 0;  // La última vez que el rebote del botón fue manejado

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
    for (int tiempo = tiempoInicial; tiempo > 0; tiempo--) {
        Serial.println(tiempo);
        delay(1000);  // Esperar un segundo
    }
    Serial.println("¡El mundo ha sido destruido!");
}

void setup() {
    Serial.begin(9600);
    Serial.println("Inicio del programa");
}

void loop() {
    unsigned long currentTime = millis();

    if (currentTime - lastDebounceTime > DEBOUNCE_DELAY) {
        if (Serial.available() > 0) {
            char input = Serial.read();
            if (input == 'u') {
                tiempoInicial++;
                Serial.print("Tiempo ajustado a: ");
                Serial.println(tiempoInicial);
                lastDebounceTime = currentTime;
            } else if (input == 'd') {
                tiempoInicial = max(1, tiempoInicial - 1);  // Evita que el tiempo sea menor a 1 segundo
                Serial.print("Tiempo ajustado a: ");
                Serial.println(tiempoInicial);
                lastDebounceTime = currentTime;
            } else if (input == 's') {
                cuenta_regresiva(); // Inicia la cuenta regresiva
                // Aquí podrías resetear tiempoInicial o realizar otra lógica necesaria después de la cuenta regresiva
                lastDebounceTime = currentTime;
            }
        }
    }

    // La siguiente sección de leer y verificar la clave se mantiene inalterada.
    // Suponiendo que quieras seguir leyendo una secuencia para alguna otra funcionalidad, 
    // aquí deberías incluir esa lógica.
    // Recuerda que este código está diseñado para demostrar el uso de botones para ajustar 
    // y comenzar una cuenta regresiva. Dependiendo de tus necesidades específicas, 
    // es posible que debas ajustar cómo y cuándo lees la entrada del usuario o señales externas.
}


