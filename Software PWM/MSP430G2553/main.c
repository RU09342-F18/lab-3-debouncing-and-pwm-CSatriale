#include <msp430.h>

int on = 125;  //ccr on value
int off = 125; //ccr off value
int onoff = 0; //switch condition for on and off 

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;        //stops watchdog timer
    TA0CCTL0 = CCIE;                 //enables capture compare
    TA0CCR0 = 125;                   //(4MHz / 4)(1/1kHz) = 250, so 125 on and 125 off creates 50% duty cycle @ 1kHz
    TA0CTL = TASSEL_2 + MC_1 + ID_2; //sets parameters for clock


    P1DIR = BIT0 + BIT6;  //sets pin1.0 and pin1.6 to output


    P1OUT =  BIT3;        //P1.3 pullup
    P1REN |= BIT3;        //enables P1.3 resistor
    P1IE |= BIT3;         //enables P1.3 interrupt
    P1IES |= BIT3;        //interrupt generates on the high low
    P1IFG &= ~BIT3;       //flag set off at pin1.3

    __bis_SR_register(LPM0_bits + GIE);  //enters lowpowermode0 during interrupt
    }

#pragma vector=TIMER0_A0_VECTOR   //interrupt triggers on timer A0.0
__interrupt void BUTTON(void){
    if (on == 250){               //ccr value met
        P1OUT &= ~BIT0;           //LED value 0, off all the way
    } else {
        P1OUT ^= BIT0;            //else the LED toggles through duty cycles
    }
    if (onoff == 0){              //If onoff is off/0
    TA0CCR0 = off;                //CCR0 set to ccr off
    } else {
    TA0CCR0 = on;                 //else CCR0 set to ccr on
    }
    onoff = ~onoff;               //onoff toggles on and off (0xffff / 0x0000);
}

#pragma vector=PORT1_VECTOR       //interrupt triggers on button press
__interrupt void Port1(void){
          on = on - 25;           //on ccr value decreased by 10%
          off = off + 25;         //off value increased by the decrease amount
          if (on < 0){            //higher limit
              on = 250;           //values have 0% duty cycle
              off = 0;
          }
          P1IFG &= ~BIT3;         //Button flag turned off
}
