@ EWRAM variables not yet defined in C, in address order, as pret's
@ sym files list them. Each .space runs to the next variable.
	.section .sym_ewram,"aw",%nobits
	.space 0x00000040
	.global GameFlagBytes
GameFlagBytes:
	.space 0x00000200
	.global gPartyState
gPartyState:
	.space 0x00005610
	.global Sound_Work
Sound_Work:
	.space 0x00000fb0
	.global Sound_CommandTable
Sound_CommandTable:
	.space 0x00000004
	.global Sound_JumpCommand
Sound_JumpCommand:
	.space 0x0000008c
	.global Sound_CgbNotes
Sound_CgbNotes:
	.space 0x000002c0
	.global Sound_WorkBytes
Sound_WorkBytes:
	.space 0x000009d2
	.global gScrollTarget
gScrollTarget:
