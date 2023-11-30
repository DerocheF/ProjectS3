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
/*! \brief data */
unsigned char data;
/*! \brief flag */
unsigned char flag = 0;
int main(void) {
  USART_Init(MYUBRR);
  sei();
  while (1) {
    _delay_ms(1);
    //USART_Transmit(USART_Receive()+1);
    //USART_Transmit(flag+48);
    if (flag == 1) {
      //USART_Transmit(data);
      USART_putsln("Bonjour");
      flag = 0;
    }
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
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  /*! \brief Set frame format: 8data, 1stop bit,No parity */
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
  DDRB = 0b0010000;
  PORTB = 0b0;
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
/*! \brief Transmit
 *
 *  This function is used to receive a char.
 *
 */
unsigned char USART_Receive(void) {
  /* Wait for data to be received */
  while (!(UCSR0A & (1 << RXC0)))
    ;
  /* Get and return received data from buffer */
  return UDR0;
}
ISR(USART_RX_vect) {
  data = UDR0;
  flag = 1;
  PORTB = 0b0010000;
}
void USART_puts(unsigned char *str) {
  do {
    USART_Transmit(*str);
  } while (*++str != 0);
}
void USART_putsln(unsigned char *str) {
  USART_puts(str);
  USART_Transmit('\n');//ou 10 en ASCii
  USART_Transmit('\r');//ou 13 en ASCii
}

