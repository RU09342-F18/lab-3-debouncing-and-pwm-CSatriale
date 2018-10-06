#include <msp430.h>

int ccr1 = 500; //ccr1 value

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;        //holds watchdog timer
   
    P1DIR |= BIT0 + BIT6;  //sets pin1.0 and pin1.6 to output
    P1SEL |= BIT6;        //P1.6 set as PWM
    P1OUT =  BIT3;        //P1.3 pullup
    P1REN |= BIT3;        //enables P1.3 resistor
    P1IE |= BIT3;         //enables P1.3 interrupt
    P1IES |= BIT3;        //interrupt generates on the high low
    P1IFG &= ~BIT3;       //flag set off at pin1.3
    
    TA0CCR0 = 1000;       //1kHz period
    TA0CCTL1 = OUTMOD_7;  //reset/set timer
    TA0CCR1 = ccr1;       //ccr1 50% duty cycle
    TA0CTL = TASSEL_2 + MC_1; //sets parameters for clock
     
     __bis_SR_register(LPM0_bits + GIE);  //enters lowpowermode0 during interrupt
    }

#pragma vector=PORT1_VECTOR       //interrupt triggers on button press
__interrupt void Button(void){
          P1OUT ^= BIT0;    //debugging toggle
 if (ccr1 == 1000){        //CCR1 at max
        ccr1 = 0;          //sets ccr1 back to 0
    } else {                            
        ccr1 = ccr1 + 100;   // increment ccr1 by 10%
    }
    TA0CCR1 = ccr1;          //sets ccr1 for duty cycle
    P1IFG = ~BIT3;           // interrupt flag off
}
