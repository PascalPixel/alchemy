	.section .rodata
	.balign 4
@ 仲間 8 人
	.global gCharacter_DefinitionTable
gCharacter_DefinitionTable:
	.incbin "games/tbs/src/game/chars.table"
