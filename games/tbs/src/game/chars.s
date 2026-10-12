	.section .rodata
	.balign 4
@ 仲間 8 人
	.global Character_DefinitionTable
Character_DefinitionTable:
	.incbin "game/chars.table"
