/**
 * 5 semestre - Eng. da Computação - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 0 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Introduzir ASF e HAL
 *  - Configuracao de clock
 *  - Configuracao pino In/Out
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 */

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

#define LED_PIO				PIOC					//Perifério que controla o LED
#define LED_PIO_ID			ID_PIOC						//ID do periférico PIOC que controla o LED
#define LED_PIO_IDX			8						//ID do LED no PIO
#define LED_PIO_IDX_MASK	(1 << LED_PIO_IDX)		//Máscara para controlarmos o LED

#define BUT_PIO				PIOA					//Perifério que controla o botão SW300
#define BUT_PIO_ID			ID_PIOA						//ID do periférico PIOA que controla o botão SW300
#define BUT_PIO_IDX			11						//ID do botão SW300 no PIO
#define BUT_PIO_IDX_MASK	(1 << BUT_PIO_IDX)		//Máscara para controlarmos o botão SW300
/************************************************************************/
/* constants                                                            */
/************************************************************************/

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void init(void);

/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/

// Função de inicialização do uC
void init(void)
{
	// Inicializa o board clock
	sysclk_init();
	
	//Desativa o cão de guarda do timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	//Ativa o PIO na qual o LED foi conectado para que possamos controlar o LED
	pmc_enable_periph_clk(LED_PIO_ID);
	
	//Inicializa PCB como saída
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
	
	//Inicializa o PIO do botão
	pmc_enable_periph_clk((BUT_PIO_ID));
	
	//inicializa PCB como entrada
	pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
	
	//PULL-UP
	pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, 1);
}

void _pio_set(Pio *p_pio, const uint32_t ul_mask) {
	p_pio->PIO_SODR = ul_mask;
}



/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
int main(void)
{
  //Inicializa sistema e IOs
  init();
 

  // super loop	
  // aplicacoes embarcadas não devem sair do while(1).
  while (1)
  {
	pio_get(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK); //Get do BUT_PIO
	
	if (!pio_get(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK)) {
		
		for (int i = 0; i<5; i++){
			_pio_set(PIOC, LED_PIO_IDX_MASK);	//coloca 1 no pino LED
			delay_ms(200);						//Delay por software de 200 ms
			pio_clear(PIOC, LED_PIO_IDX_MASK);	//Coloca 0 no pino do LED
			delay_ms(200);						//Delay por software de 200 ms	
		}
	}
  }
  return 0;
}
