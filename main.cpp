#include <iostream>
#include <stdio.h>

static int table[] = {
	0xceb1, 0xceb2, 0xcf87, 0xceb4, 0xceb5, 0xcf86, 0xceb3,
	0xceb7, 0xceb9, 0x0000, 0xceba, 0xcebb, 0xcebc, 0xcebd, 0xcebf,
	0xcf80, 0xcf88, 0xcf81, 0xcf83, 0xcf84, 0xcf85, 0xcf82, 0xcf89,
	0xceb3, 0xceb8, 0xceb6 
};

// Convert Greek text copied from Online Bible to unicode utf-8.
// Copies stdin to stdout, converting as it goes.
// Prefixes the output with the utf-8 BOM
// Recommended usage:
// 1. Copy the input text with cmd-C from an Online Bible window.
// 2. use the Terminal command
//		OLB2UTF > ~/Desktop/zzz.txt
// 3. Paste the input text into the terminal window.
// 4. Type ctrl-D to signal end of file
//
// The act of pasting into the terminal window apparently converts
// high-ascii characters from the font encoding into utf-8.
int main (int argc, char * const argv[]) {
	// convert stdin to stdout
	char ch;
	int uch;
	char inbuf[256];
	char outbuf[512];
	outbuf[0] = 0xef;		// write utf-8 BOM
	outbuf[1] = 0xbb;
	outbuf[2] = 0xbf;
	fwrite(outbuf, 1, 3, stdout);
	
	size_t num(0);
	while (num = fread(inbuf, 1, 256, stdin)) {
		// convert num characters to output buffer
		char *inP = inbuf;
		char *outP = outbuf;
		while (num--) {
			ch = *inP++;
			if (ch < 0x61 || ch > 0x7a || ch == 'j') {
				*outP++ = ch;
			} else {
				// convert from table
				uch = table[ch-0x61];
				*outP++ = (uch >> 8) & 0xff;
				*outP++ = uch & 0xff;
			}
		}
		num = outP - outbuf;
		fwrite(outbuf, 1, num, stdout);
	}
    return 0;
}



// read a file a buffer at a time until end of file.
// for each buffer, convert and write to output file.