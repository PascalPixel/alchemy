	.section .rodata
	.balign 4
@ Definitions of the 203 classes.
	.global gClass_DefinitionTable
gClass_DefinitionTable:
	.incbin "games/tbs/src/game/classes.table"
