@ Name entry keyboard: same as the Japanese gNamingScreen_KeyboardTilemap.
@ Compressed tilemap; unreferenced, but retained in this overlay in all six editions.
	.section .rodata
	.balign 4
	.global MainMenu_KeyboardTilemap
MainMenu_KeyboardTilemap:
	.incbin "games/tbs/src/graphics/window/keyboard_ja.table.lz"
	.balign 4, 0
