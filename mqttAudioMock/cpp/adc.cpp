#include "mbed.h"
#include "MAX9814.h"

#define MAX_BUF_SIZE 128

Serial pc (USBTX, USBRX);

AnalogIn mic(A0);

char ad_data_buf0[MAX_BUF_SIZE];
char ad_data_buf1[MAX_BUF_SIZE];
int i = 0;
char buf_sel = 0;

void adcTickfunc() {
    
    if (buf_sel == 0) {
        ad_data_buf0[i] = mic.read_u16() >> 4;
    }
    else { 
        ad_data_buf1[i] = mic.read_u16() >> 4; 
    }
    
    ++i;
    
    // 100usec delay
    // wait(10);
    
    pc.printf("\n\r pwm %d", ad_data_buf0[i]);

    if (i >= MAX_BUF_SIZE) { 
        i = 0;
        
        if (buf_sel){
            buf_sel = 0;
        } 
        else {
            buf_sel = 1;
        }
    }
}

int main()
{
    while (1) {
        adcTickfunc();
    }
}