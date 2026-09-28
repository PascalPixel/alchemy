@ IWRAM variables not yet defined in C, in address order, as pret's
@ sym files list them. Each .space runs to the next variable.
	.section .sym_iwram_high,"aw",%nobits
	.space 0x0000045c
	.global Data_03007800
Data_03007800:
	.space 0x00000004
	.global Data_03007804
Data_03007804:
	.space 0x000007f8
	.global Data_03007ffc
Data_03007ffc:
