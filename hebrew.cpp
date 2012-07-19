/*
 *  hebrew.cpp
 *  OLB2UTF
 *
 *  Created by Bob Sander-Cederlof on 3/10/12.
 *  Copyright 2012 Adobe Systems (Richardson). All rights reserved.
 *
 */

#include "hebrew.h"

#include <iostream>
#include <stdio.h>
#include <stdint.h>

// Convert Greek or Hebrew text copied from Online Bible to unicode utf-8.
// Copies stdin to stdout, converting as it goes.
// Prefixes the output with the utf-8 BOM
// Recommended usage:
// 1. Copy the input text with cmd-C from an Online Bible window.
// 2. use the Terminal command
//		OLB2UTF G > ~/Desktop/greek.txt
//		OLB2UTF H > ~/Desktop/hebrew.txt
// 3. Paste the input text into the terminal window.
// 4. Type ctrl-D to signal end of file
//
// The act of pasting into the terminal window apparently converts
// high-ascii characters from the font encoding into utf-8.


/*
 *  hebrew.cpp
 *  OLB2UTF
 *
 *  Created by Bob Sander-Cederlof on 3/9/12.
 *  Copyright 2012 Adobe Systems (Richardson). All rights reserved.
 *
 */

static uint32_t finals[] = {
	0xd79a,	//	K 4B kaf fnl		05da	d7 9a
	0,
	0xd79d,	//	M 4D mem fnl		05dd d7 9d
	0xd79f,	//	N 4E nun fnl		05df d7 9f
	0,
	0xd7a3,	//	P 4F pe fnl		05e3 d7 a3
	0,0,0,0,
	0xd7a5	//	U 55 tsadi fnl	05e5 d7 a5
};
//	
static uint32_t hebrew[] = {
	0x0000d790, //	a 61 aleph		05D0 D7 90
	0x0000d791, //	b 62 beth		05d1 d7 91
	0x00000000, //	c 63 --
	0x0000d793, //	d 64 daleth		05d3 d7 93
	0x0000d7a2, //	e 65 ayin		05e2 d7 a2
	0x00000000, //	f 66 --
	0x0000d792, //	g 67 gimel		05d2 d7 92
	0x0000d794, //	h 68 he			05d4 d7 94
	0x00000000, //	i 69 --
	0x0000d798, //	j 6a tet		05d8 d7 98
	0x0000d79b, //	k 6b kaf		05db d7 9b
	0x0000d79c, //	l 6c lamed		05dc d7 9c
	0x0000d79e, //	m 6d mem		05de d7 9e
	0x0000d7a0, //	n 6e nun		05e0 d7 a0
	0x0000d7a1, //	o 6f samech		05e1 d7 a1
	0x0000d7a4, //	p 70 pe			05e4 d7 a4
	0x0000d7a7, //	q 71 qof		05e7 d7 a7
	0x0000d7a8, //	r 72 resh		05e8 d7 a8
	0x00efacab, //	s 73 sin		fb2b ef ac ab
	0x0000d7aa, //	t 74 tav		05ea d7 aa
	0x0000d7a6, //	u 75 tsadi		05e6 d7 a6
	0x00efacaa, //	v 76 shin 		fb2a ef ac aa
	0x0000d795, //	w 77 vav		05d5 d7 95
	0x0000d797, //	x 78 cheth		05d7 d7 97
	0x0000d799, //	y 79 yod		05d9 d7 99
	0x0000d796, //	z 7a zayin		05d6 d7 96
};

enum {
	kNotHebrew, kHebrew
};

// =============================================================================
// ============================= Internal Routines =============================
// =============================================================================

static char *ConvertRun(char *runStart, int runLen, char *outP);
static char *CopyRun(char *runStart, int runLen, char *outP);
static bool GetNextRun(char *linebuf, int start, int len, int &runLen, int &runType);
static void ReverseRun(char *runStart, int runLen);

// -----------------------------------------------------------------------------
// *ConvertRun
// -----------------------------------------------------------------------------
static char *ConvertRun(char *runStart, int runLen, char *outP)
{
	// loop over the characters from begin to end
	uint32_t value(0);
	for (int i=0; i<runLen; i++) {
		char ch = runStart[i];
		// skip entirely if linefeed or carriage return
		//		if (ch == '\n' || ch == '\r')
		//			continue;
		
		if (ch >= 0x4b && ch <= 0x55) {
			// final forms
			value = finals[ch-0x4b];
		}
		else if (ch >= 0x61 && ch <= 0x7a) {
			value = hebrew[ch-0x61];
		}
		else {
			value = 0;
		}
		
		if (value) {
			if (value > 0x0000ffff) {
				*outP++ = (value >> 16) & 0xff;
			}
			*outP++ = (value >> 8) & 0xff;
			ch = value & 0xff;
		}
		
		*outP++ = ch;
	}
	
	return outP;
}

// -----------------------------------------------------------------------------
// *CopyRun
// -----------------------------------------------------------------------------
static char *CopyRun(char *runStart, int runLen, char *outP)
{
	for (int i=0; i<runLen; i++) {
		*outP++ = *runStart++;
	}
	return outP;
}

// -----------------------------------------------------------------------------
// GetNextRun
// -----------------------------------------------------------------------------
static bool GetNextRun(char *linebuf, int start, int len, int &runLen, int &runType)
{
	runLen = 0;
	runType = kNotHebrew;
	char firstChar = linebuf[start];
	if (firstChar >= 'a' && firstChar <= 'z') {
		// hebrew run, find end
		runType = kHebrew;
	}
	
	for (int i=start; i<len; i++) {
		char ch = linebuf[i];
		int charType = kNotHebrew;
		if ((ch >= 'a' && ch <= 'z')
			|| ch == 'K' || ch == 'M' || ch == 'N' || ch == 'P' || ch == 'U') {
			charType = kHebrew;
		} else if (ch == '-' || ch == ':' || ch == ' ') {
			charType = runType;
		}
		
		if (charType != runType) {
			break;
		}
		runLen++;
	}
	
	//	if (runType == kHebrew && runLen > 0) {
	//		if (linebuf[start + runLen - 1] == ' ') {
	//			runLen--;
	//		}
	//	}
	return runLen > 0;
}

// -----------------------------------------------------------------------------
// ReverseRun
// -----------------------------------------------------------------------------
static void ReverseRun(char *runStart, int runLen)
{
	char *leftP = runStart;
	char *rightP = runStart + runLen - 1;
	while (leftP < rightP) {
		char ch = *leftP;
		*leftP++ = *rightP;
		*rightP-- = ch;
	}
}

// =============================================================================
// ============================= External Routines =============================
// =============================================================================

// -----------------------------------------------------------------------------
// convertHebrew
// -----------------------------------------------------------------------------
int convertHebrew () {
	// convert stdin to stdout
	char outbuf[512];
	outbuf[0] = 0xef;		// write utf-8 BOM
	outbuf[1] = 0xbb;
	outbuf[2] = 0xbf;
	fwrite(outbuf, 1, 3, stdout);
	
	// we will read a line at a time
	size_t len(0);
	char *linebuf(0);
	while ((linebuf = fgetln(stdin, &len))) 
	{
		// trim cr and lf from end of line if any
		while (len > 0 && (linebuf[len-1] == '\n' || linebuf[len-1] == '\r')) {
			len--;
		}
		// reverse the rest of the line to get Hebrew in correct order
		ReverseRun(linebuf, len);
		int i = 0;
		int runLen(0);
		int runType(0);
		char *outP = outbuf;
		while(GetNextRun(linebuf, i, len, runLen, runType)) {
			if (runType == kHebrew) {
				outP = ConvertRun(linebuf + i, runLen, outP);
			} else {
				// reverse non-Hebrew runs to get back to correct order
				ReverseRun(linebuf+i, runLen);
				outP = CopyRun(linebuf + i, runLen, outP);
				*outP++ = ' ';
			}
			i += runLen;
		}
		*outP++ = '\n';
		len = outP - outbuf;
		fwrite(outbuf, 1, len, stdout);
		
		*outP++ = 0;
//		fprintf(stderr, "%s", outbuf);
	}
	return 0;
}
