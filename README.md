# Interfaces-Comunica-o-

Projeto de Controle de LEDs e Display OLED

Este projeto implementa um sistema embarcado para controle de LEDs e exibição de informações em um display OLED SSD1306. O código foi desenvolvido para funcionar com um microcontrolador que utiliza PIOs para controle de hardware e comunicação via I2C com o display.

Funcionalidades

Controle de LEDs RGB utilizando interrupções de botões.

Exibição de mensagens no display OLED.

Manipulação de desenhos no display.

Entrada via USB para alteração do padrão de exibição.

Estrutura do Código

General.h: Definições gerais e configurações.

Leds.h: Controle de LEDs.

ssd1306.h e font.h: Biblioteca para controle do display OLED.

Funções principais:

InitHardware(): Inicializa os periféricos e configurações iniciais.

SetInterruption(int pin): Configura as interrupções dos botões.

HandleInterruption(uint gpio, uint32_t events): Manipula eventos de botão.

UpdateLed(uint gpio, volatile uint32_t *lastTime): Alterna o estado dos LEDs.

LedInformationMessage(uint gpio, bool ledStatus): Exibe mensagens sobre LEDs.

UpdateDrawing(int number): Atualiza os desenhos no display.

UpdateDisplay(char c, uint8_t x, uint8_t y): Atualiza o display com informações.

HandleInput(char c): Trata a entrada de caracteres via USB.

Como Utilizar

Compile e carregue o código no microcontrolador.

Pressione os botões para alternar o estado dos LEDs.

Conecte via USB e envie caracteres para alterar os desenhos exibidos.

Observe as mensagens no display OLED e no terminal.

Dependências

Biblioteca ssd1306 para controle do display OLED.

Biblioteca de comunicação via I2C.

Configuração de GPIOs e interrupções para botões.

Este projeto pode ser expandido para incluir mais funcionalidades, como animações no display ou comunicação sem fio.

Video: https://drive.google.com/file/d/15GyyNqk2YrhgHZyPPc84xQMczCjb6tNU/view?usp=sharing
