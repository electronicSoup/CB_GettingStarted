/**
 *
 * libesoup/examples/main_barebones.c
 *
 * An example main.c file for the bare bones minimum required to use the
 * libesoup library of code.
 * 
 * The code is used in the example MPLAB-X project:
 * libesoup/examples/projects/microchip/BareBones.X
 *
 * Copyright 2018 electronicSoup Limited
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the version 2 of the GNU Lesser General Public License
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *******************************************************************************
 *
 */
#include "libesoup_config.h"
#include "libesoup/timers/delay.h"
#include "libesoup/timers/sw_timers.h"
#include "libesoup/gpio/gpio.h"
#include "libesoup/comms/uart/uart.h"

#define DEBUG_FILE
static const char *TAG = "MAIN";
#include "libesoup/logger/serial_log.h"

#define SN65HVD72D


#ifdef MAX3221
#define MAX3221E_RX          RD1
#define MAX3221E_INVALID     RD2
#define MAX3221E_TX          RD3
#define MAX3221E_FORCEON     RD4
#define MAX3221E_FORCEOFF    RD5
#define MAX3221E_ENABLE      RD6
#endif

#ifdef SN65HVD72D
#define SN65HVD72D_TX        RD0
#define SN65HVD72D_TX_ENABLE RD1
#define SN65HVD72D_RX        RD2

#define SN65HVD72D_RECEIVE   0b0
#define SN65HVD72D_SEND      0b1
#endif

#define RC_CHECK_STOP        if (rc <0) while (1);

static struct uart_data   uart;
static uint8_t test_buffer[] = "Test";

void exp_fn(timer_id timer, union sigval data)
{
	result_t rc;
	
#ifdef SN65HVD72D
	rc = gpio_set(SN65HVD72D_TX_ENABLE, GPIO_MODE_DIGITAL_OUTPUT, SN65HVD72D_SEND);
	RC_CHECK_STOP
#endif
	rc = uart_tx_buffer(&uart, test_buffer, 4);
	RC_CHECK_STOP
}

#ifdef SN65HVD72D
void tx_finished(void *data)
{
	result_t rc;
	
	rc = gpio_set(SN65HVD72D_TX_ENABLE, GPIO_MODE_DIGITAL_OUTPUT, SN65HVD72D_RECEIVE);
	RC_CHECK_STOP	
}
#endif

void rx_char(uint8_t ch)
{
	LOG_D("rx_char(%c)\n\r", ch);
}

int main()
{
	result_t          rc;
	struct timer_req  request;

	rc = libesoup_init();
	RC_CHECK_STOP

#ifdef MAX3221
	rc = gpio_set(MAX3221E_RX, GPIO_MODE_DIGITAL_INPUT, 0);
	RC_CHECK_STOP

	rc = gpio_set(MAX3221E_TX, GPIO_MODE_DIGITAL_OUTPUT, 0);
	RC_CHECK_STOP

	rc = gpio_set(MAX3221E_ENABLE, GPIO_MODE_DIGITAL_OUTPUT, 0);
	RC_CHECK_STOP

	rc = gpio_set(MAX3221E_FORCEON, GPIO_MODE_DIGITAL_OUTPUT, 1);
	RC_CHECK_STOP

	rc = gpio_set(MAX3221E_FORCEOFF, GPIO_MODE_DIGITAL_OUTPUT, 1);
	RC_CHECK_STOP

	rc = gpio_set(MAX3221E_INVALID, GPIO_MODE_DIGITAL_INPUT, 0);
	RC_CHECK_STOP
#endif

#ifdef SN65HVD72D
	rc = gpio_set(SN65HVD72D_TX, GPIO_MODE_DIGITAL_OUTPUT, 0);
	RC_CHECK_STOP
		
	rc = gpio_set(SN65HVD72D_TX_ENABLE, GPIO_MODE_DIGITAL_OUTPUT, SN65HVD72D_RECEIVE);
	RC_CHECK_STOP
		
	rc = gpio_set(SN65HVD72D_RX, GPIO_MODE_DIGITAL_INPUT, 0);
	RC_CHECK_STOP
#endif
	/*
	 * Initialise the UART connected to the MAX3221E
	 */
	rc = uart_calculate_mode(&uart.uart_mode, UART_8_DATABITS, UART_PARITY_NONE, UART_ONE_STOP_BIT, UART_IDLE_HIGH);
	RC_CHECK_STOP

#ifdef MAX3221
	uart.tx_pin = MAX3221E_TX;
	uart.rx_pin = MAX3221E_RX;
#endif
#ifdef SN65HVD72D
	uart.tx_pin = SN65HVD72D_TX;
	uart.rx_pin = SN65HVD72D_RX;
	uart.tx_finished = tx_finished;
#endif
	uart.baud = 9600;                // Nice relaxed baud rate
	uart.process_rx_char = rx_char;
	
	/*
	 * Reserve a UART channel for our use
	 */
	rc = uart_reserve(&uart);
	RC_CHECK_STOP
		
	request.units          = Seconds;
	request.duration       = 10;
	request.type           = repeat;
	request.exp_fn         = exp_fn;
	request.data.sival_int = 0;
	
	rc = sw_timer_start(&request);
        
	while(1) {
		CHECK_TIMERS()
		Nop();
	}
}
