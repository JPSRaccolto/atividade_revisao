#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"
#include "ws2812.pio.h"


#define BOTAO_A 5
#define AZUL 12
#define VERDE 11
#define VERMELHO 13
#define VERTICALY 26
#define HORIZONTALX 27
#define SELEC 22
#define PWM_WRAP 4095
#define PWM_CLK_DIV 30.52f
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO_I2C 0x3C
#define buzzer1 10
#define buzzer2 21
#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7

bool cor = 0;
bool controle_leds = true;
absolute_time_t last_interrupt_time = 0;
int captura = 0;
bool vitoria = false;
bool derrota = false;
bool buzzer_state_1 = false;
bool buzzer_state_2 = false;
bool som_ativo = false;
uint32_t buzzer_timer = 0;
// Variável global correta para o display
ssd1306_t ssd;
// Buffer para armazenar quais LEDs estão ligados matriz 5x5

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

int i = 0;

double desenho0[25] = {
    0.0, 0.2, 0.2, 0.2, 0.0,
    0.2, 0.0, 0.0, 0.0, 0.2,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.2, 0.0, 0.0, 0.0, 0.2,
    0.2, 0.0, 0.0, 0.0, 0.2
};

void num0(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho0[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}

// Desenho do número 1
double desenho1[25] = 
    {0.2, 0.0, 0.0, 0.0, 0.2,
     0.0, 0.2, 0.2, 0.2, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.2, 0.0, 0.0, 0.0, 0.2,
     0.2, 0.0, 0.0, 0.0, 0.2};

void num1(uint8_t r, uint8_t g, uint8_t b){
    
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);  
     // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (desenho1[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
    
}

//labirinto desenhado
const uint8_t labirinto[8][16] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1},
    {1,0,1,1,0,1,0,0,0,1,1,1,1,1,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,1,0,1,1,1,0,0,0,1,1,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,1,1,1,1,1,1,1,1,0,1,1,1,0,1},
    {1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0}
};

//configurações do PWM
void pwm_init_gpio(uint gpio, uint wrap, float clkdiv) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, wrap);
    pwm_config_set_clkdiv(&config, clkdiv);
    pwm_init(slice_num, &config, true);
}

// Mapeamento Y 
int16_t mapeamento_Y(uint16_t valor_y) {
    int16_t valor_mapeado;
    if (valor_y < 2048) {
        valor_mapeado = 2048 - valor_y;
    } else if (valor_y > 2048) {
        valor_mapeado = valor_y - 2048;
    } else {
        valor_mapeado = 0;
    }
    return valor_mapeado;
}

// Mapeamento X 
int16_t mapeamento_x(uint16_t valor_x) {
    int16_t valor_mapeado_x;
    if (valor_x < 2048) {
        valor_mapeado_x = 2048 - valor_x;
    } else if (valor_x > 2048) {
        valor_mapeado_x = valor_x - 2048;
    } else{
        valor_mapeado_x = 0;
    }
    return valor_mapeado_x;
}

void tocar_nota(uint pin, float freq, int duracao_ms) {
  if (freq == 0) {
      pwm_set_gpio_level(pin, 0);
      sleep_ms(duracao_ms);
      return;
  }

  uint slice = pwm_gpio_to_slice_num(pin);
  uint32_t clock_freq = 125000000 / PWM_CLK_DIV;
  uint32_t top = (uint32_t)(clock_freq / freq);
  pwm_set_wrap(slice, top);
  pwm_set_gpio_level(pin, top / 2);  // 50% duty cycle
  sleep_ms(duracao_ms);
  pwm_set_gpio_level(pin, 0);
  sleep_ms(50); // pausa entre notas
}

void tocar_vitoria(uint pin) {
  float notas[] = {523.25, 659.25, 783.99, 1046.5}; // Dó, Mi, Sol, Dó agudo
  int duracoes[] = {200, 200, 200, 400};

  for (int i = 0; i < 4; i++) {
      tocar_nota(pin, notas[i], duracoes[i]);
  }
}


void tocar_derrota(uint pin) {
  float notas[] = {523.25, 392.00, 261.63}; // Dó, Sol grave, Dó grave
  int duracoes[] = {300, 300, 600};

  for (int i = 0; i < 3; i++) {
      tocar_nota(pin, notas[i], duracoes[i]);
  }
}

//interrupção e debounce
void gpio_callback(uint gpio, uint32_t events) {
  absolute_time_t now = get_absolute_time();
  if (absolute_time_diff_us(last_interrupt_time, now) < 250000) return;
  last_interrupt_time = now;

  if (gpio == SELEC) {
    if (vitoria || derrota) {
        num0(0,0,0);
        vitoria = false;
        derrota = false;
        ssd1306_fill(&ssd, false);
        ssd1306_send_data(&ssd);
        printf("Jogo reiniciado\n");
    }
  } else if (gpio == BOTAO_A) {
    som_ativo = !som_ativo;
    printf("Som %s\n", som_ativo ? "ativado" : "desativado");
  }
}
//inicia pinos
void inicia_pinos() {
  stdio_init_all();
  sleep_ms(50);
  PIO pio = pio0;
  int sm = 0;
  uint offset = pio_add_program(pio, &ws2812_program);

  ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
  pwm_init_gpio(buzzer1, PWM_WRAP, PWM_CLK_DIV);
  pwm_set_gpio_level(buzzer1, 0);  // Inicialmente desligado

  pwm_init_gpio(buzzer2, PWM_WRAP, PWM_CLK_DIV);
  pwm_set_gpio_level(buzzer2, 0);  // Inicialmente desligado

  adc_init();
  adc_gpio_init(VERTICALY);
  adc_gpio_init(HORIZONTALX);

  gpio_init(VERDE);
  gpio_set_dir(VERDE, GPIO_OUT);
  gpio_init(AZUL);
  gpio_set_dir(AZUL, GPIO_OUT);
  gpio_init(VERMELHO);
  gpio_set_dir(VERMELHO, GPIO_OUT);


  gpio_init(SELEC);
  gpio_set_dir(SELEC, GPIO_IN);
  gpio_pull_up(SELEC);

  gpio_init(BOTAO_A);
  gpio_set_dir(BOTAO_A, GPIO_IN);
  gpio_pull_up(BOTAO_A);

  gpio_set_irq_enabled_with_callback(SELEC, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}
//desenha o labirinto
void labirinto_inicia(int pos_x, int pos_y) {
  const int square_size = 8;
  int linha = pos_y / square_size;
  int coluna = pos_x / square_size;
  for (int linha = 0; linha < 8; linha++) {
      for (int coluna = 0; coluna < 16; coluna++) {
          if (labirinto[linha][coluna] == 1) {
              int x = coluna * square_size;
              int y = linha * square_size;
              ssd1306_rect(&ssd, y, x, square_size, square_size, true, true);
          }
      }
  }

  ssd1306_rect(&ssd, pos_y, pos_x, square_size, square_size, true, true);
//logica para definir se perdeu ou ganhou
  if (labirinto[pos_y / square_size][pos_x / square_size] == 1) {
    num1(255,255,0);
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "VOCE PERDEU", 20, 30);
    ssd1306_send_data(&ssd);  
    printf("Você perdeu! Seu personagem bateu na parede!\n");
    derrota = !derrota;
    if(derrota == 1){
      gpio_put(VERMELHO, 1);
      if (som_ativo) {
          tocar_derrota(buzzer1);
          tocar_derrota(buzzer2);
      }
      sleep_ms(3000);
      gpio_put(VERMELHO, 0);
  }
  }

  else if (((linha == 6 && (coluna == 1 || coluna == 10 || coluna == 14))) && labirinto[linha][coluna] == 0) {
      num0(255,0,0);
      ssd1306_fill(&ssd, false);
      ssd1306_draw_string(&ssd, "VOCE VENCEU", 20, 30);
      ssd1306_send_data(&ssd);
      printf("Parabéns! Você venceu o jogo!\n");
      vitoria = !vitoria;
      if(vitoria == 1){
          gpio_put(VERDE, 1);
          if (som_ativo) {
              tocar_vitoria(buzzer1);
              tocar_vitoria(buzzer2);
          }
          sleep_ms(3000); 
          gpio_put(VERDE, 0);
      }
      
  }

}
//inicia o display
void inicializa_display() {
  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA);
  gpio_pull_up(I2C_SCL);

  ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO_I2C, I2C_PORT);
  ssd1306_config(&ssd);
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);
}

int main() {
  inicializa_display();
  inicia_pinos();

  const int square_size = 8;
  int centro_x = (WIDTH - square_size) / 2;
  int centro_y = (HEIGHT - square_size) / 2;

  while (true) {
    adc_select_input(0);
    uint16_t valor_y = adc_read();
    adc_select_input(1);
    uint16_t valor_x = adc_read();


    int pos_x = centro_x + (((int)valor_x - 2048) * centro_x) / 2048;
    int pos_y = centro_y + ((2048 - (int)valor_y) * centro_y) / 2048;

    labirinto_inicia(pos_x, pos_y);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    sleep_ms(50);
  }
}
