/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

#include "xparameters.h"
#include "xgpio.h"
#define LED_DELAY (100000000)

//#define XPAR_AXI_GPIO_0_DEVICE_ID 0
#define XPAR_AXI_GPIO_1_DEVICE_ID 1
#define XPAR_AXI_GPIO_2_DEVICE_ID 2

int main()
{
	//Giving structs to each of its coressponding one such as LED, RGB_LED and Switches.
	XGpio LED_Gpio, RGB_LED_Gpio, SWITCH_Gpio;
	int Status;
	init_platform();
	// This is for the GPIO Ports such as Switches, RGB LED, and LEDS
	Status = XGpio_Initialize(&LED_Gpio, XPAR_AXI_GPIO_0_DEVICE_ID);
	if (Status != XST_SUCCESS){
		xil_printf("GPIO has failed.\r\n");
		return XST_FAILURE;
	}
	Status = XGpio_Initialize(&RGB_LED_Gpio, XPAR_AXI_GPIO_1_DEVICE_ID);
	if (Status != XST_SUCCESS){
		xil_printf("RGB has failed.\r\n");
		return XST_FAILURE;
	}
	Status = XGpio_Initialize(&SWITCH_Gpio, XPAR_AXI_GPIO_2_DEVICE_ID);
	if (Status != XST_SUCCESS){
		xil_printf("SWITCH has failed.\r\n");
		return XST_FAILURE;
	}

	XGpio_SetDataDirection(&LED_Gpio,1,0x00);
	XGpio_SetDataDirection(&RGB_LED_Gpio,1,0x00);
	XGpio_SetDataDirection(&SWITCH_Gpio,1,0xFF);

	int sw = 0; // Starting Value will be at zero
	int ring_counter = 1; // Starting value will be 1 because the ring counter starts from 1,2,4,8,1,...etc.
	int binary_counter = 0; // Starting Value will be at zero
	while(1)
	{
		// Bit masking the bottom four bits so that we can control the SWITCHES
		sw = XGpio_DiscreteRead(&SWITCH_Gpio, 1) & 0xF;
		switch(sw)
		{
		//RGB LED should be RED
		case 0x1:
            XGpio_DiscreteWrite(&LED_Gpio,1, 0x1);
            XGpio_DiscreteWrite(&RGB_LED_Gpio, 1, 0x4);
            ring_counter = 1;
            binary_counter = 0;
			break;
		//RGB LED should be GREEN
		case 0x2:
			XGpio_DiscreteWrite(&LED_Gpio,1, 0x2);
			XGpio_DiscreteWrite(&RGB_LED_Gpio, 1, 0x2);
			ring_counter = 1;
			binary_counter =0;
			break;
		//RGB LED should be BLUE
		case 0x4:
			XGpio_DiscreteWrite(&LED_Gpio,1,0x4);
			XGpio_DiscreteWrite(&RGB_LED_Gpio,1,0x1);
			ring_counter = 1;
			binary_counter = 0;
			break;
		//RGB LED should be WHITE (R+G+B)
		case 0x8:
			XGpio_DiscreteWrite(&LED_Gpio,1,0x8);
			XGpio_DiscreteWrite(&RGB_LED_Gpio,1,0x7);
			ring_counter = 1;
			binary_counter = 0;
			break;
		//RGB LED should be OFF, this is the Binary Counter
		case 0x3:
			XGpio_DiscreteWrite(&LED_Gpio,1,binary_counter);
			XGpio_DiscreteWrite(&RGB_LED_Gpio,1,0x0);
			binary_counter = (binary_counter + 1) & 0xF;
			for (volatile int i = 0; i < LED_DELAY; i++);
			break;
		//RGB LED should be OFF, this is the Ring Counter
		case 0xC:
			XGpio_DiscreteWrite(&LED_Gpio, 1, ring_counter);
			XGpio_DiscreteWrite(&RGB_LED_Gpio,1,0x0);
			ring_counter = ring_counter << 1;
			if (ring_counter > 0x8)
				ring_counter = 1;
		    for (volatile int i = 0; i < LED_DELAY; i++);
			break;
		//RGB LED should be OFF
		default:
			XGpio_DiscreteWrite(&LED_Gpio,1,0x00);
			XGpio_DiscreteWrite(&RGB_LED_Gpio,1,0x00);
			ring_counter=1;
			binary_counter=0;
			break;
		}
	}
	return 0;
}
