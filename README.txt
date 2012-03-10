OLB2UTF is a very simple Terminal command to convert Greek or Hebrew text copied from Mac Online Bible.
Online Bible (OLB) uses OLB Greek and OLB Hebrew fonts, which are not unicode based.
If you want to use the text in a program which uses Unicode, you have to convert. 
Otherwise you get gibberish.

Convert Greek or Hebrew text copied from Online Bible to unicode utf-8
Copies stdin to stdout, converting as it goes.
Prefixes the output with the utf-8 BOM.
Recommended usage:
1. Copy the input text with cmd-C from an Online Bible window.
2. use the Terminal command
		OLB2UTF G > ~/Desktop/greek.txt
		OLB2UTF H > ~/Desktop/hebrew.txt
3. Paste the input text into the terminal window.
4. Type ctrl-D to signal end of file
The act of pasting into the terminal window converts high-ascii characters 
like the paragraph symbol from the font encoding into utf-8.
This program will then convert the ascii codes used by OLB to the Greek or Hebrew Unicode equivalent.
For Hebrew, the program will also attempt to re-order the characters on each line 
so that the apparent right-to-left flow is maintained for the Hebrew characters. 
So far I have not figured out a perfect algorithm for the re-ordering. 
I think I have it correct for the Hebrew characters, 
but the non-Hebrew characters such as verse numbers may be "not quite right".