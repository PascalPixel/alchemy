	.section .rodata
	.balign 4
@ 244 classes.
	.global gClass_DefinitionTable
gClass_DefinitionTable:
	.incbin "games/tla/src/game/character/classes.table"
