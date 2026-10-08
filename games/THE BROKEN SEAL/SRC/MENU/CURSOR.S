	.section .rodata
	.balign 4
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "MENU/CURSOR_L.8bpp8x16"
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "MENU/CURSOR_R.8bpp8x16"
	.global Menu_AnimatedCursorTiles
Menu_AnimatedCursorTiles:
	.incbin "MENU/CURSPIN.8bpp16x16"
