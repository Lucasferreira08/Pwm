// #include <stdio.h>
// #include "pico/stdlib.h"

// int main()
// {
//     stdio_init_all();

//     while (true) {
//         printf("Hello, world!\n");
//         sleep_ms(1000);
//     }
// }

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22       // GPIO para o servomotor
#define LED_PIN 12         // GPIO para o LED RGB
#define PWM_FREQ 50        // Frequência de 50 Hz
#define CLOCK_DIV 64       // Divisor de clock para ajuste fino
#define WRAP_VALUE 39062   // Valor de wrap para 50Hz (125MHz / (64 * 50) ≈ 39062.5)

void setup_pwm() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM); // Configura GPIO como PWM
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN); // Obtém o slice associado
    
    // Configura frequência do PWM
    pwm_set_clkdiv(slice, CLOCK_DIV); // Define divisor de clock
    pwm_set_wrap(slice, WRAP_VALUE);  // Define valor de wrap
    pwm_set_enabled(slice, true);     // Habilita o slice PWM
}

void set_servo_position(uint pulse_us) {
    // Converte microssegundos para valor de canal PWM
    float duty = (pulse_us / 20000.0f) * WRAP_VALUE; // 20000µs = 20ms (período)
    pwm_set_gpio_level(SERVO_PIN, (uint16_t)duty);
}

int main() {
    stdio_init_all(); // Inicializa comunicação
    setup_pwm();      // Configura PWM para o servo
    
    // Controle do LED RGB (BitDogLab)
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    while (true) {
        // Posição 180 graus (2400µs)
        set_servo_position(2400);
        gpio_put(LED_PIN, 1); // LED ligado
        sleep_ms(5000);
        
        // Posição 90 graus (1470µs)
        set_servo_position(1470);
        gpio_put(LED_PIN, 0); // LED desligado
        sleep_ms(5000);
        
        // Posição 0 graus (500µs)
        set_servo_position(500);
        gpio_put(LED_PIN, 1); // LED ligado
        sleep_ms(5000);
        
        // Movimento suave entre 0 e 180 graus
        for (int pulse = 500; pulse <= 2400; pulse += 5) {
            set_servo_position(pulse);
            sleep_ms(10); // Atraso para movimento suave
        }
        for (int pulse = 2400; pulse >= 500; pulse -= 5) {
            set_servo_position(pulse);
            sleep_ms(10); // Atraso para movimento suave
        }
    }
}