/*! \file *********************************************************************
 *
 * \brief
 *      Motor control implementation.
 *
 *      This file contains the full implementation of the motor control,
 *      except the PID-controller.
 *
 * \par Application note:
 *      AVR447: Sinusoidal driving of three-phase permanent motor using
 *      ATmega48/88/168
 *
 * \par Documentation
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      DEROCHE Flavien
 *
 * $Name: RELEASE_1_0 $
 * $Revision: 1.8 $
 * $RCSfile: main.c,v $
 * $Date: 2006/03/27 07:20:51 $  \n
 ******************************************************************************/
/*! \brief Clock Speed */
#define FOSC 16000000
/*! \brief Baud Rate */
#define BAUD 9600
/*! \brief UBRR */
#define MYUBRR FOSC / 16 / BAUD - 1

int main(void) {
  USART_Init(MYUBRR);
  while (1) {
    _delay_ms(100);
    USART_Transmit('A');
  }
}

/*! \brief Initialisation
 *
 *  This function is used to init USART
 *
 *  \param ubrr
 */
void USART_Init(unsigned int ubrr) {
  /*! \brief Set baud rate */
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  /*! \brief Enable transmitter */
  UCSR0B = (1 << TXEN0);
  /*! \brief Set frame format: 8data, 1stop bit,No parity */
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
/*! \brief Transmit
 *
 *  This function is used to transfer a char.
 *
 *  \param data data to transfer.
 */
void USART_Transmit(unsigned char data) {
  /*! \brief Wait for empty transmit buffer */
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  /*! \brief Put data into buffer, sends the data */
  UDR0 = data;
}
