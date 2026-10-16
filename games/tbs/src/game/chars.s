	.section .rodata
	.balign 4
@ Definitions of the eight party members.
	.global gCharacter_DefinitionTable
gCharacter_DefinitionTable:
	.incbin "games/tbs/src/game/chars.table"
