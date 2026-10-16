	.section .rodata
	.global Data_0812a16c
Data_0812a16c:
	.byte 4, 7, 5, 0x80, 0x81
	.balign 4, 0
@ Debug party members (separated by owner -1).
@ The Broken Seal's gDebug_PartyPresets, with six item columns for its four
	.global gData_0812a174
gData_0812a174:
	.incbin "games/tla/src/debug/presets.table"
