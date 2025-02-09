#include "General.h"
#include "Leds.h"
#include "include/ssd1306.h"
#include "include/font.h"
#include <stdio.h>

// Variáveis globais
refs pio;                               // Referência do PIO
uint32_t valorLed;                      // Valor do LED a ser enviado
RGB color = {2, 4, 8};                  // Cor do LED (RGB)
double *drawing;                        // Desenho (sequência de LEDs acesos)
static volatile uint32_t lastTimeA = 0; // Tempo de última interrupção do botão A
static volatile uint32_t lastTimeB = 0; // Tempo de última interrupção do botão B
ssd1306_t ssd;

// Prototipação de funções
void SetInterruption(int pin);
void HandleInterruption(uint gpio, uint32_t events);
void UpdateLed(uint gpio, volatile uint32_t *lastTime);
void LedInformationMessage(uint gpio, bool ledStatus);
void UpdateDrawing(int number);
void HandleInput(char c);
void UpdateDisplay(char c, uint8_t x, uint8_t y);
void InitHardware();

int main()
{
    InitHardware();
    
    while (true)
    {
        if (stdio_usb_connected())
        {
            char c;
            scanf("%c", &c);
            HandleInput(c);
            sleep_ms(1000);
        }
    }
}

// Inicializa hardware e configurações
void InitHardware()
{
    pio = InitPIO();
    SetInput(BUTTON_A);
    SetInterruption(BUTTON_A);
    SetInput(BUTTON_B);
    SetInterruption(BUTTON_B);
    SetOutput(GREEN_LED);
    SetOutput(BLUE_LED);
    
    drawing = Drawing(0);
    Draw(drawing, valorLed, pio, color);
    
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADRESS, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    UpdateDisplay(' ', 0, 0);
}

// Configuração de interrupção para botões
void SetInterruption(int pin)
{
    gpio_set_irq_enabled(pin, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_callback(&HandleInterruption);
    irq_set_enabled(IO_IRQ_BANK0, true);
}

// Lida com interrupções dos botões
void HandleInterruption(uint gpio, uint32_t events)
{
    if (gpio == BUTTON_A) UpdateLed(GREEN_LED, &lastTimeA);
    else if (gpio == BUTTON_B) UpdateLed(BLUE_LED, &lastTimeB);
}

// Atualiza o estado do LED com base na interrupção
void UpdateLed(uint gpio, volatile uint32_t *lastTime)
{
    uint32_t currentTime = to_us_since_boot(get_absolute_time());
    if (currentTime - *lastTime > 250000)
    {
        *lastTime = currentTime;
        gpio_put(gpio, !gpio_get(gpio));
        LedInformationMessage(gpio, gpio_get(gpio));
    }
}

// Exibe mensagens sobre o estado dos LEDs
void LedInformationMessage(uint gpio, bool ledStatus)
{
    const char *msg = (gpio == GREEN_LED) ? (ledStatus ? "LED VERDE ON" : "LED VERDE OFF") : (ledStatus ? "LED AZUL ON" : "LED AZUL OFF");
    printf("%s\n", msg);
    UpdateDisplay(' ', 0, 0);
    ssd1306_draw_string(&ssd, msg, 8, 46);
    ssd1306_send_data(&ssd);
}

// Atualiza o desenho exibido
void UpdateDrawing(int number)
{
    drawing = Drawing(number);
    Draw(drawing, valorLed, pio, color);
}

// Atualiza o display
void UpdateDisplay(char c, uint8_t x, uint8_t y)
{
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
    ssd1306_draw_string(&ssd, "Ariel CEPEDI", 8, 10);
    ssd1306_draw_char(&ssd, c, x, y);
    ssd1306_send_data(&ssd);
}

// Lida com entrada do usuário
void HandleInput(char c)
{
    UpdateDisplay(c, 8, 28);
    if (c >= '0' && c <= '9') UpdateDrawing(c - '0');
}
