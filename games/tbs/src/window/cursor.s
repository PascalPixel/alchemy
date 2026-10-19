	.section .rodata
	.balign 4
	.global gMenu_CursorLeftObjectTiles
gMenu_CursorLeftObjectTiles:
	.incbin "games/tbs/src/menu/cursor_l.8bpp8x16"
	.global gMenu_CursorObjectTiles
gMenu_CursorObjectTiles:
	.incbin "games/tbs/src/menu/cursor_r.8bpp8x16"
	.global gMenu_AnimatedCursorTiles
gMenu_AnimatedCursorTiles:
	.incbin "games/tbs/src/menu/cursor_spin.8bpp16x16"
