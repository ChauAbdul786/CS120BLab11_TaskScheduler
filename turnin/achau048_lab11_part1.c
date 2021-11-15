/*	Author: Abdullah Chaudhry
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
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


    switch(state){
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


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;

    /* Insert your solution below */
    task tasks[1];
    unsigned char i = 0;
    unsigned char numTasks = 1;
    tasks[i].state = Start_Keypad;
    tasks[i].period = 50;
    tasks[i].elapsedTime = 50;
    tasks[i].TickFct = &TickFct_Keypad;
   

    TimerSet(50);
    TimerOn();

    while (1) {
        for (i = 0; i < numTasks; i++){
            if(tasks[i].elapsedTime == tasks[i].period){
                tasks[i].state = tasks[i].TickFct(tasks[i].state);
                tasks[i].elapsedTime = 0;
            }
            tasks[i].elapsedTime += 50;
        }

        while(!TimerFlag) {};
        TimerFlag = 0;
    }
    return 1;
}
