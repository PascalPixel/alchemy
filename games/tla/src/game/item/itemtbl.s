	.section .rodata
	.balign 4
@ 道具 516 個
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "games/tla/src/game/item/items.table"
