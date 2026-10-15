	.section .rodata
	.balign 4
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "games/common/src/menu/cursor_l.8bpp8x16"
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "games/common/src/menu/cursor_r.8bpp8x16"
	.global Menu_AnimatedCursorTiles
Menu_AnimatedCursorTiles:
	.incbin "games/common/src/menu/cursor_spin.8bpp16x16"
