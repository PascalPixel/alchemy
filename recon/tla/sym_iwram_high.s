@ IWRAM variables not yet defined in C, in address order, as pret's
@ sym files list them. Each .space runs to the next variable.
	.section .sym_iwram_high,"aw",%nobits
	.space 0x00000c1b
	.global Data_03006fbf
Data_03006fbf:
	.space 0x00000001
	.global IwramSoundMixWorkspace
IwramSoundMixWorkspace:
	.space 0x00000840
	.global gCartridgeResetMarker
gCartridgeResetMarker:
	.space 0x00000004
	.global Data_03007804
Data_03007804:
	.space 0x000007ec
	.global Data_03007ff0
Data_03007ff0:
	.space 0x0000000c
	.global Data_03007ffc
Data_03007ffc:
