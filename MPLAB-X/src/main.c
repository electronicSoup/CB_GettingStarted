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
//#include "libesoup/timers/hw_timers.h"
#include "libesoup/timers/sw_timers.h"

#define DEBUG_FILE
static const char *TAG = "MAIN";
#include "libesoup/logger/serial_log.h"

#define MAX3221E_RX          RD1
#define MAX3221E_INVALID     RD2
#define MAX3221E_TX          RD3
#define MAX3221E_FORCEON     RD4
#define MAX3221E_FORCEOFF    RD5
#define MAX3221E_ENABLE      RD6

void exp_fn(timer_id timer, union sigval data)
{
	LATDbits.LATD3 = ~PORTDbits.RD3;
}

int main()
{
	result_t          rc;
	struct timer_req  request;

	TRISDbits.TRISD0 = 0;
	LATDbits.LATD0 = 0;
	
	TRISDbits.TRISD3 = 0;
	LATDbits.LATD3 = 0;

	rc = libesoup_init();
	if(rc < 0) {
		// ERROR
		LATDbits.LATD0 = 1;
	}
	
	delay(Seconds, 2);
	if(rc < 0) {
		// ERROR
		LATDbits.LATD0 = 1;
	}

	request.units          = mSeconds;
	request.duration       = 200;
	request.type           = repeat;
	request.exp_fn         = exp_fn;
	request.data.sival_int = 0;
	
	LATDbits.LATD3 = 1;
	rc = sw_timer_start(&request);
	if(rc < 0) {
		// ERROR
		LATDbits.LATD0 = 1;
	}
        
//        serial_log(LOG_DEBUG, TAG, "Hello World\n\r");
        LOG_D("Entering main loop\n\r");
        LOG_I("Information message\n\r");
        LOG_W("Warning!\n\r");
        LOG_E("ERROR!!!\n\r");

	while(1) {
		CHECK_TIMERS()
		// 25mS peocessing !
		Nop();
	}
}
