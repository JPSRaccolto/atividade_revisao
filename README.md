![image](https://github.com/user-attachments/assets/f2a5c9b8-6208-4723-8f46-1d74be421827)


# 🛠️ Projeto: ADC, PWM, UART e I2C com RP2040

## 📑 Sumário
- [🎯 Objetivos](#-objetivos)
- [📋 Descrição do Projeto](#-descrição-do-projeto)
- [⚙️ Funcionalidades Implementadas](#%EF%B8%8F-funcionalidades-implementadas)
- [🛠️ Requisitos do Projeto](#%EF%B8%8F-requisitos-do-projeto)
- [📂 Estrutura do Repositório](#-estrutura-do-reposit%C3%A1rio)
- [🖥️ Como Compilar](#%EF%B8%8F-como-compilar)
- [🤝 Contribuições](#-contribui%C3%A7%C3%B5es)
- [📽️ Demonstração em Vídeo](#%EF%B8%8F-demonstra%C3%A7%C3%A3o-em-v%C3%ADdeo)
- [📜 Licença](#-licen%C3%A7a)
- [💡 Considerações Finais](#-considera%C3%A7%C3%B5es-finais)

## 🎯 Objetivos
• Revisar o funcionamento do conversor analógico-digital (ADC) no RP2040.

• Utilizar o PWM para controlar a intensidade e frequência sonora dos buzzers.

• Representar a posição do joystick no display SSD1306 por meio de um quadrado móvel.

• Aplicar o protocolo de comunicação I2C na integração com o display ssd1306.

• Aplicar o protocolo de comunicação UART para transmissão de dados e mensagens de depuração.

• Utilizar a matriz de LED, LED RGB e buzzers para feedback visual.

• Implementar soluções para botões com interrupções e debounce.

## 📋 Descrição do Projeto
Este projeto utiliza a placa BitDogLab com os seguintes componentes:
- JoyStick (GPIOs 26, 27, 22)
- LED RGB (GPIOs 11 e 13)
- Botão A (GPIO 5)
- Display SSD1306 via I2C (GPIO 14 e GPIO 15)
- Matriz de LED 5x5 (GPIO 7)
- Buzzers (GPIO 10 e 21)
  
## ⚙️ Funcionalidades Implementadas
1. **Conversor Analógico-Digital:**
   - Converter os valores recebidos do JoyStick.
     
2. **Modulação PWM dos Buzzers:**
   - Altera a intensidade sonora dos buzzers.
     
3. **Acionamento de interrupção com o Botão A e o Botão do Joystick:**
   - Interrompe o PWM dos buzzers ao ser precionado e o botão do Joystick reinicia ao ser pressionado.

4. **Display ssd1306:**
   - Apresenta um espelho da posição do JoyStick. Ademais, apresenta paredes que formam um labirinto.

5. **Feedback Visual:**     
   - Apresenta um rosto triste ou feliz na matriz de LED a depender da vitória ou derrota do usuário.

6. **Quadrado de 8x8 pixels:**
   - Exibido do display ssd1306.
     
## 🛠️ Requisitos do Projeto
- **Uso de Interrupções:** Controlar os eventos dos botões.
- **Debouncing:** Implementação via software.
- **Feedback Visual:** Via Buzzers, LED RGB e Matriz de LED.
- **Uso do Display SSD1306:** Exibição do Joystick e labirinto que comunica via I2C.
- **Organização do Código:** Código estruturado e comentado.
- **Uso da UART:** Para transmissão de dados de mensagens de depuração.
- **Leitura analógica:** Via ADC.
- 
## 📂 Estrutura do Repositório
```
├── Atividade_1_fase_2.c             # Código principal do projeto
└── ssd1306.c            # Configuração do controle do display
└── ssd1306.h            # Configura a .c como biblioteca
└── ws2812.pio.h         # Configura o controle da matriz de LED
└── font.h               # Configura as letras e números para o dispaly ssd1306
└── ...                  # Demais arquivos necessários
```

## 🖥️ Como Compilar
1. Clone o repositório:
   ```bash
   https://github.com/JPSRaccolto/atividade_revisao.git
   ```
2. Navegue até o diretório do projeto:
   ```bash
   cd atividade_revisao
   ```
3. Compile o projeto com seu ambiente de desenvolvimento configurado para o RP2040.
4. Carregue o código na placa BitDogLab.

## 🖥️ Metodo alternativo:
1. Baixe o repositório com arquivo zip.
2. Extraia para uma pasta de fácil acesso
3. Utilize a extensão raspberry pi pico dentro do VS Code para importar o projeto.
4. Aguarde ate criar o arquivo build
5. Utilize o ícone "_compile_" para compilar.
6. Utilize o "_RUN_" com a BitDogLab em modo boot seel para enviar o programa para a sua RP2040.
7. Agora, interaja com os botões e o teclado para mergulhar nas funcionalidades do projeto.

## 🧑‍💻 Autor
**João Pedro Soares Raccolto**

## 📝 Descrição
Tarefa apresentada ao Cepedi como parte dos critérios de avaliação do curso EmbarcaTech em Software e Sistemas Embarcados, com foco na aplicação prática de comunicação serial, PWM, I2C, ADC e integração de hardware com o microcontrolador RP2040.

## 🤝 Contribuições
Este projeto foi desenvolvido por **João Pedro Soares Raccolto**.
Contribuições são bem-vindas! Siga os passos abaixo para contribuir:

1. Fork este repositório.
2. Crie uma nova branch:
   ```bash
   git checkout -b minha-feature
   ```
3. Faça suas modificações e commit:
   ```bash
   git commit -m 'Minha nova feature'
   ```
4. Envie suas alterações:
   ```bash
   git push origin minha-feature
   ```
5. Abra um Pull Request.

## 📽️ Demonstração em Vídeo
- O vídeo de demonstração do projeto pode ser visualizado aqui: [Drive](https://drive.google.com/file/d/1PxKWCw_stxOIgTNSCgIACTfDbgvxRU92/view?usp=sharing)

## 💡 Considerações Finais
Este projeto oferece uma ótima oportunidade para consolidar conhecimentos sobre conversor analógico/digital, manipulação de hardware, UART,
I2C, PWM e desenvolvimento com microcontroladores. Certifique-se de seguir todos os requisitos e manter um código limpo e bem comentado.
