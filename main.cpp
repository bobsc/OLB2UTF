/*
 *  main.cpp
 *  OLB2UTF
 *
 *  Created by Bob Sander-Cederlof on 3/9/12.
 *  Copyright 2012. All rights reserved.
 *
 */
#include <iostream>
#include <stdio.h>
#include <stdint.h>

#include "greek.h"
#include "hebrew.h"

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




// =============================================================================
// ============================= External Routines =============================
// =============================================================================
int main (int argc, char * const argv[]) {
	if (argc < 2) {
		printf("%s OLB to UTF Conversion\n", argv[0]);
		printf("Convert Greek or Hebrew text copied from Online Bible to unicode utf-8\n");
		printf("Copies stdin to stdout, converting as it goes.\n");
		printf("Prefixes the output with the utf-8 BOM.\n");
		printf("Recommended usage:\n");
		printf("1. Copy the input text with cmd-C from an Online Bible window.\n");
		printf("2. use the Terminal command\n");
		printf("		OLB2UTF G > ~/Desktop/greek.txt\n");
		printf("		OLB2UTF H > ~/Desktop/hebrew.txt\n");
		printf("3. Paste the input text into the terminal window.\n");
		printf("4. Type ctrl-D to signal end of file\n");
		printf("The act of pasting into the terminal window apparently converts\n");
		printf("high-ascii characters like the paragraph symbol from the font encoding into utf-8.\n");
		return 1;
	}
	char key = argv[1][0];
	if (key == 'g' || key == 'G') {
		convertGreek();
	} else {
		convertHebrew();
	}
	return 0;
}