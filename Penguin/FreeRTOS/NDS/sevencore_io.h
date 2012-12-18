/*****************************************************************
*
*                     Virtual Input/Output Devices
*                   --------------------------------
*           (C) Copyright 2009 by Minsuk Lee, Seoul, Korea
*                      All Rights Reserved.
*
*****************************************************************/

/* FILE        : sevencore_io.c
 *
 * Description : Virtual Input/Output Definition
 *
 * Created - 2010-04-20 by Minsuk Lee
 * Revised - 2010-08-15 by Dukjoon Chun - filled some detail
 *           2010-11-20 by Minsuk Lee   - Source Re-Factoring
 *                                      - Includes wrapping for real IO
 *
 */

#ifndef NDS_IO_H_
#define NDS_IO_H_

#include <queue.h>


extern int init_printf(void);
#define printf	iprintf

// Key Matrix API
//extern xQueueHandle KeyQueue;
#define MAX_KEY_LOG		10

void key_init(void);
int  kbhit(void);
u8   getkey(void);

//-------------------------------------------
// NDS Real I/O Wrapping

// Return Value of KeyCurrent()
#define NDS_SWITCH()	(keysCurrent() & 0xFFFF)

#define KEY_A		0x0001
#define KEY_B		0x0002
#define KEY_X		0x0400
#define KEY_Y		0x0800
#define KEY_L		0x0200
#define KEY_R		0x0100
#define KEY_START	0x0008
#define KEY_SELECT	0x0004
#define KEY_UP		0x0040
#define KEY_DOWN	0x0080
#define KEY_LEFT	0x0020
#define KEY_RIGHT	0x0010

#define MSEC2TICK(msec)		((int)(msec / portTICK_RATE_MS))

#endif /* NDS_IO_H_ */
