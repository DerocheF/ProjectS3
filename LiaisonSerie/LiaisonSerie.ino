/*! \file *********************************************************************
 * \author
 *      DEROCHE Flavien
 ******************************************************************************/

/** \file
 *  \brief Implémentation du contrôle moteur.
 *
 *  Ce fichier contient l'implémentation complète du contrôle moteur,
 *  à l'exception du régulateur PID.
 */

/** \mainpage
 *  \par Note d'application
 *  AVR447 : Conduite sinusoïdale d'un moteur permanent triphasé à l'aide
 *  d'ATmega48/88/168
 *
 *  \par Documentation
 *  Pour une documentation complète du code, les compilateurs pris en charge,
 *  les paramètres du compilateur et les dispositifs pris en charge, voir readme.html
 *
 *  \author DEROCHE Flavien
 *
 *  \version $Name: RELEASE_1_0 $
 *  \version $Revision: 1.8 $
 *  \version $RCSfile: main.c,v $
 *  \version $Date: 2006/03/27 07:20:51 $
 */

#define FOSC 16000000               /*!< \brief Fréquence de l'horloge */
#define BAUD 9600                   /*!< \brief Taux de baud de la communication série */
#define MYUBRR FOSC / 16 / BAUD - 1 /*!< \brief Valeur d'UBRR */

unsigned char data;      /*!< \brief Donnée transmise et reçue */
unsigned char flag0 = 0; /*!< \brief Indicateur pour USART0 */
unsigned char flag3 = 0; /*!< \brief Indicateur pour USART3 */

/*! \brief Fonction principale
 *
 *  Cette fonction initialise l'USART et exécute le code principal.
 */

/*! \brief Fonction principale.
 *
 *  Cette fonction initialise l'USART et exécute la boucle principale.
 *
 */
int main(void) {
  // Initialisation de l'USART avec la valeur de l'UBRR calculée
  USART_Init(MYUBRR);

  // Activation des interruptions globales
  sei();

  // Boucle principale
  while (1) {
    // Si l'indicateur flag3 est activé
    if (flag3) {
      // Transmet la donnée via USART0
      USART0_Transmit(data);
      // Réinitialise l'indicateur
      flag3 = 0;
    }

    // Si l'indicateur flag0 est activé
    if (flag0) {
      // Transmet la donnée via USART3
      USART3_Transmit(data);
      // Réinitialise l'indicateur
      flag0 = 0;
    }
    // Pause d'1 milliseconde
    _delay_ms(1);
  }
}


/*! \brief Initialisation de l'USART.
 *
 *  Configure les registres de l'USART.
 *
 *  \param ubrr Valeur d'UBRR
 */

void USART_Init(unsigned int ubrr) {
  /* Configuration du taux de baud */
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  /* Activation du récepteur et du transmetteur */
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  /* Format de trame : 8 données, 1 bit de stop */
  UCSR0C = (1 << UCSZ00)| (1 << UCSZ01);

  /* Configuration du taux de baud */
  UBRR3H = (unsigned char)(ubrr >> 8);
  UBRR3L = (unsigned char)ubrr;
  /* Activation du récepteur et du transmetteur */
  UCSR3B = (1 << RXEN3) | (1 << TXEN3) | (1 << RXCIE3);
  /* Format de trame : 8 données, 1 bit de stop */
  UCSR3C = (1 << UCSZ30)| (1 << UCSZ31);
}

/*! \brief Routine de service d'interruption de réception USART0.
 *
 *  Gère la réception de données sur USART0.
 */

ISR(USART0_RX_vect) {
  data = UDR0;
  flag0 = 1;
}
/*! \brief Routine de service d'interruption de réception USART3.
 *
 *  Gère la réception de données sur USART3.
 */

ISR(USART3_RX_vect) {
  data = UDR3;
  flag3 = 1;
}

/*! \brief Fonction de transmission USART0.
 *
 *  Envoie un octet de données via USART0.
 *
 *  \param data Donnée à transmettre
 */

void USART0_Transmit(unsigned char data) {
  /* Attente du tampon de transmission vide */
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  /* Place la donnée dans le tampon, envoie la donnée */
  UDR0 = data;
}

/*! \brief Fonction de réception USART0.
 *
 *  Reçoit un octet de données via USART0.
 *
 *  \return Donnée reçue
 */
void USART3_Transmit(unsigned char data) {
  /* Attente du tampon de transmission vide */
  while (!(UCSR3A & (1 << UDRE3)))
    ;
  /* Place la donnée dans le tampon, envoie la donnée */
  UDR3 = data;
}

/*! \brief Fonction de réception USART3.
 *
 *  Reçoit un octet de données via USART3.
 *
 *  \return Donnée reçue
 */
unsigned char USART0_Receive(void) {
  /* Attente de la réception de données */
  while (!(UCSR0A & (1 << RXC0)))
    ;
  /* Récupère et renvoie la donnée reçue du tampon */
  return UDR0;
}

/*! \brief Fonction USART0_puts.
 *
 *  Lit et transmet une chaîne de caractères.
 *  Pointe vers chaque caractère de la chaîne.
 *
 *  \param str Chaîne de caractères à transmettre
 */

unsigned char USART3_Receive(void) {
  /* Attente de la réception de données */
  while (!(UCSR3A & (1 << RXC3)))
    ;
  /* Récupère et renvoie la donnée reçue du tampon */
  return UDR3;
}

/*! \brief Fonction USART3_puts.
 *
 *  Lit et transmet une chaîne de caractères.
 *  Pointe vers chaque caractère de la chaîne.
 *
 *  \param str Chaîne de caractères à transmettre
 */


void USART0_puts(unsigned char *str) {
  do {
    USART0_Transmit(*str);
  } while (*++str != 0);
}

/*! \brief Fonction USART0_putsln.
 *
 *  Similaire à USART0_puts mais avec un saut de ligne après chaque transmission.
 *
 *  \param str Chaîne de caractères à transmettre
 */

void USART0_putsln(unsigned char *str) {
  USART0_puts(str);
  USART0_Transmit('\n');
  USART0_Transmit('\r');
}

/*! \brief Fonction USART3_puts.
 *
 *  Lit et transmet une chaîne de caractères.
 *  Pointe vers chaque caractère de la chaîne.
 *
 *  \param str Chaîne de caractères à transmettre
 */
void USART3_puts(unsigned char *str) {
  do {
    USART3_Transmit(*str);
  } while (*++str != 0);
}

/*! \brief Fonction USART3_putsln.
 *
 *  Similaire à USART3_puts mais avec un saut de ligne après chaque transmission.
 *
 *  \param str Chaîne de caractères à transmettre
 */

void USART3_putsln(unsigned char *str) {
  USART3_puts(str);
  USART3_Transmit('\n');
  USART3_Transmit('\r');
}

/*! \brief Fonction de transmission USART3.
 *
 *  Envoie un octet de données via USART3.
 *
 *  \param data Donnée à transmettre
 */
