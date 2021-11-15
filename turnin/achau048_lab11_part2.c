/*	Author: Abdullah Chaudhry
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #11  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://drive.google.com/file/d/1hl3_ELGnU9ySy03nNXt5SC1sm0EXcFbS/view?usp=sharing
 *
 */
#include <avr/io.h>
#include "timer.h"
#include "keypad.h"
#include "scheduler.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char temp = 0x00;
unsigned char keypad = 0x00;
enum Keypad {Start_Keypad, Idle};
int TickFct_Keypad(int state){
    temp = GetKeypadKey();
    switch(state){ //State Transitions
    case Start_Keypad:     
        state = Idle; 
        break;
    
    case Idle:  
        state = Idle;   
        break;
        
    default:    
        state = Start_Keypad;
        break;
    }


    switch(state){ //State Actions
    case Start_Keypad: 
        break;

    case Idle:  
        switch(temp){
        case '\0':  
            keypad = 0x1F; 
            break;
            
        case '1':   
            keypad = 0x01; 
            break;
            
        case '2':   
            keypad = 0x02; 
            break;
            
        case '3':   
            keypad = 0x03; 
            break;
            
        case '4':   
            keypad = 0x04; 
            break;
            
        case '5':   
            keypad = 0x05; 
            break;
            
        case '6':   
            keypad = 0x06; 
            break;
            
        case '7':   
            keypad = 0x07; 
            break;
            
        case '8':   
            keypad = 0x08; 
            break;
            
        case '9':   
            keypad = 0x09; 
            break;
            
        case 'A':   
            keypad = 0x0A; 
            break;
            
        case 'B':   
            keypad = 0x0B; 
            break;
            
        case 'C':   
            keypad = 0x0C; 
            break;
            
        case 'D':   
            keypad = 0x0D; 
            break;
            
        case '*':   
            keypad = 0x0E; 
            break;
            
        case '0':   
            keypad = 0x00; 
            break;
            
        case '#':   
            keypad = 0x0F; 
            break;
            
        default:    
            keypad = 0x1F; 
            break;
        }

        PORTB = keypad;
        break;
                
        default:    
            break;
    }
    return state;
}

enum LCD { LCD_Start, LCD_Idle };
unsigned char text[] = "            CS120B is Legend... wait for it DARY!";
unsigned char currPosition = 0x00;
unsigned char lcdScreen[16];
int TickFct_LCD(int state) {
    switch(state){ //State Transitions
    case LCD_Start:  
        state = LCD_Idle;
        break;
        
    case LCD_Idle:   
        state = LCD_Idle;
        break;
        
    default:    
        state = LCD_Start;
        break;
    }

    switch(state){ //State Actions
    case LCD_Start:  
        break;
    case LCD_Idle:   
        for (int k = 0; k < 16; ++k) {
            lcdScreen[k] = text[(k + currPosition) % 52];
        }

        currPosition = (currPosition + 1) % 52; 
        LCD_DisplayString(1, lcdScreen);
        break;

        default:   
            break;
    }
    return state;
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xFF; PORTA = 0x00;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;
    DDRD = 0xFF; PORTD = 0x00;

    /* Insert your solution below */
    LCD_init();
    static task task1, task2;
    task *tasks[] = { &task1, &task2 };
    const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
    const char start = 0;

    task1.state = start;
    task1.period = 10;
    task1.elapsedTime = task1.period;
    task1.TickFct = &TickFct_Keypad;

    task2.state = start;
    task2.period = 20;
    task2.elapsedTime = task2.period;
    task2.TickFct = &TickFct_LCD;

    TimerSet(10);
    TimerOn();

    unsigned char i;
    while (1) {
        for (i = 0; i < numTasks; i++) {
            if (tasks[i]->elapsedTime == tasks[i]->period) {
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime = 0;
            }
            tasks[i]->elapsedTime += 1;
        }

        while(!TimerFlag) {};
        TimerFlag = 0;
    }
    return 1;
}