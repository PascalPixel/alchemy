	.section .rodata
	.balign 4
@ 20 Djinn for each of four elements.
	.global gDjinn_Definitions
gDjinn_Definitions:
	.incbin "games/tla/src/game/djinn/djinn.table"
