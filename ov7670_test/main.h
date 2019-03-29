#pragma once 
#include "mbed.h"
#include "ov7670.h"

Serial pc(USBTX,USBRX);
Timer t;
bool new_send = false;

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);


//Camera
OV7670 camera
(
    A4,A5,            // SDA,SCL(I2C / SCCB)
    D3,D8,NC,         // VSYNC,HREF,WEN(FIFO)  
    D7,D6,D5,D4,A3,A2,A1,A0, // D7-D0,   // PortIn data        p18(P0.26),p17(P0.25),p16(P0.24),p15(P0.23),p11(P0.18),p12(P0.17),p14(P0.16),p13(P0.15)
    NC,NC,D2         // RRST,OE,RCLK
); 

//RESET
extern "C" void mbed_reset();

//Serial
char word[25];
int t1 = 0; 
int t2 = 0;
int t3 = 0;

//
void parse_cmd();
void CameraSnap();
void CameraGrab();
