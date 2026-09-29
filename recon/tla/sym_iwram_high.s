@ IWRAM variables not yet defined in C, in address order, as pret's
@ sym files list them. Each .space runs to the next variable.
	.section .sym_iwram_high,"aw",%nobits
	.space 0x00000c1c
	.global IwramSoundMixWorkspace
IwramSoundMixWorkspace:
