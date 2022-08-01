#include <mega32.h>
#include <delay.h>

char reff[]={0xFE, 0xFD, 0xFB, 0xF7};
int keys[]={1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0};
char seg [] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
//char seg[] = {0xC0, 0xf9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};
unsigned long int temp, key_value;
unsigned long int i=0, inputcounter=0, ref = 0x01;
char val[4]={0, 0 , 0, 0} ;

unsigned long int keypad();
unsigned long int key_pressed;

// Timer 0 overflow interrupt service routine
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
// Reinitialize Timer 0 value
    TCNT0=0xA9;
    PORTA = ~ref;
    PORTB = seg[val[i]]; 
    ref = ref<<1;
    i++;
    if (i==4)
    {  
        i=0;
        ref = 0x01;
    }
}

void display(int value)
{
    temp = value;              
    val[3] = temp%16; temp = temp/16; 
    val[2] = temp%16; temp = temp/16;
    val[1] = temp%16; temp = temp/16;
    val[0] = temp%16; 
}

unsigned long int keypad()
{
while(1)
{
    int row = 0, col = -1, pos=-1; 
    for(row=0; row<4; row++)
    { 
        PORTD = reff[row];
        delay_ms(5); 
        if(PINC.0==0)
        {   
            while(PINC.0==0);
            col=0;
            break;
        }
        if(PINC.1==0)
        {   
            while(PINC.1==0);
            col=1;
            break;
        }
        if(PINC.2==0)
        {   
            while(PINC.2==0);
            col=2;
            break;
        }                
    }   
    if(col != -1)
    {            
        pos = row*3+col;
        inputcounter = ( inputcounter *10 ) + keys[pos];
        if(pos == 9){      
            key_value = ( inputcounter / 10 ) ;
            inputcounter = 0;
        }
        return key_value;
    } 
}
}

void main(void)
{
DDRD = 0xFF;    //output
PORTC = 0xFF;
DDRC = 0x00;    //input

DDRA = 0xFF;    //output
DDRB= 0xFF;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 125.000 kHz
// Mode: Normal top=0xFF
// OC0 output: Disconnected
// Timer Period: 0.696 ms

TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (1<<CS01) | (1<<CS00);
TCNT0=0xA9;
OCR0=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);

// Global enable interrupts
#asm("sei")
//
while (1)  {
  
    key_pressed = keypad(); 
    display(key_pressed);   
}
}

      
                                    



