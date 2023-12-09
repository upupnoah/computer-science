Extensions:
	.64s: x86-64 assembly code
	.64s: x86-64 disassembly
	.64ns: x86-64 assembly code with stack protection disabled
	.64nd: x86-64 disassembly with stack protection disabled

FILES

bufdemo.c
bufdemo.64s
bufdemo.64ns
bufdemo.64d
bufdemo.64nd
	Buffer overflow demonstration code

bufdemo-sp
bufdemo-nsp
	Compiled versions of bufdemo.c with/without stack protection

locate.c
	Code to print addresses of interesting memory locations

locate64

locate-all.xlsx
	Spreadsheet showing results of 5 executions of locate64
