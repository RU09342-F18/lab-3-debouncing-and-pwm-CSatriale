//Includes msp430 header file
#include <msp430.h>

int main(void)
{
 WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

 TA0CCTL0 = CCIE; // CCR0 interrupt enabled for timer a0
 TA0CCR0 = 25000; // Sets timer frequency

 P1DIR = BIT0;                             // sets pin1.0 as output
 P1OUT =  BIT3;                            // pin1.3 set, else reset
 P1REN |= BIT3;                            // pin1.3 pull up resistor enables
 P1IE |= BIT3;                             // enables pin1.3 interrupt
 P1IES |= BIT3;                            // Button P1.3 Hi/lo edge set
 P1IFG &= ~BIT3;                           // clears interrupts for bit3


 __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, smclk does not run in LPM4
}

// Port 1 interrupt service routine (Interrupt when button is pressed)
#pragma vector=PORT1_VECTOR
__interrupt void BUTTON(void)
{
    TA0CTL = TASSEL_2 + MC_1 + ID_3;  //sets the parameters for timer a0

    P1IFG &= ~BIT3;                   //clears interrupt
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TimerA0(void)
{
 P1OUT ^= BIT0;                            // P1.0 (LED) = toggle
 TA0CTL = MC_0;                            //stops timer
 TA0CTL = TACLR;                          //clears timer
}


