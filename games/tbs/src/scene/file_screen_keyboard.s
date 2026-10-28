@ Name entry screen keyboard: same as the Japanese gNameEntry_KeyboardTilemap.
@ Compressed tilemap; unreferenced, but retained in this overlay in all six editions.
	.section .rodata
	.balign 4
	.global FileScreen_KeyboardTilemap
FileScreen_KeyboardTilemap:
	.incbin "games/tbs/src/window/keyboard_ja.table.lz"
	.balign 4, 0
