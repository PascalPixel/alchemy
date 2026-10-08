	.section .rodata
	.balign 4
@ 道具 324 個
	.global Item_DefinitionTable
Item_DefinitionTable:
	.incbin "game/item/items.table"
