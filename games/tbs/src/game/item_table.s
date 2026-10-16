	.section .rodata
	.balign 4
@ Definitions of the 324 items.
	.global gItem_DefinitionTable
gItem_DefinitionTable:
	.incbin "games/tbs/src/game/items.table"
