#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "hardware/timer.h"

#define SERVO_PIN 22 
#define PWM_FREQ 50
#define CLOCK_DIV 64
#define WRAP_VALUE 39062

volatile int target_pulse = 2400; // Pulso alvo (usado na interrupção)
volatile int current_pulse = 500; // Pulso atual (atualizado suavemente)

void set_servo_position(uint pulse_us) {
    // Converte microssegundos para valor de canal PWM
    float duty = (pulse_us / 20000.0f) * WRAP_VALUE; // 20000µs = 20ms (período)
    pwm_set_gpio_level(SERVO_PIN, (uint16_t)duty);
}

// Handler do timer
void timer_callback() {
    if (current_pulse < target_pulse) 
    {
        current_pulse += 5;
    } else if (current_pulse > target_pulse) {
        current_pulse -= 5;
    }

    // Atualiza o PWM com o pulso atual
    set_servo_position(current_pulse);

    if (current_pulse>=2400 && target_pulse==2400)
    {
        target_pulse=500;
    }
    else if (current_pulse<=500 && target_pulse==500) 
    {
        target_pulse=2400;
    }   

    sleep_ms(10);
}

void setup_pwm() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);
    pwm_set_clkdiv(slice, CLOCK_DIV);
    pwm_set_wrap(slice, WRAP_VALUE);
    pwm_set_enabled(slice, true);
}

int main() {
    stdio_init_all();
    setup_pwm();
    // gpio_init(LED_PIN);
    // gpio_set_dir(LED_PIN, GPIO_OUT);

    // Posição 180 graus (2400µs)
    set_servo_position(2400);
    sleep_ms(5000);
    
    // Posição 90 graus (1470µs)
    set_servo_position(1470);
    sleep_ms(5000);
    
    // Posição 0 graus (500µs)
    set_servo_position(500);
    sleep_ms(5000);

    // Configura timer para interrupção a cada 10ms
    // repeating_timer_t timer;
    // add_repeating_timer_ms(10, timer_callback, NULL, &timer);

    while (true) 
    {
        timer_callback();
    }
}