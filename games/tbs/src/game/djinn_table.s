	.section .rodata
	.balign 4
@ Djinn definitions: 20 entries for each of the four elements.
	.global gDjinn_DefinitionTable
gDjinn_DefinitionTable:
	.incbin "games/tbs/src/game/djinn.table"
