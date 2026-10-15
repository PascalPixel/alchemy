	.section .rodata
	.balign 4
	.global gMenu_CursorLeftObjectTiles
gMenu_CursorLeftObjectTiles:
	.incbin "games/common/src/menu/cursor_l.8bpp8x16"
	.global gMenu_CursorObjectTiles
gMenu_CursorObjectTiles:
	.incbin "games/common/src/menu/cursor_r.8bpp8x16"
	.global gMenu_AnimatedCursorTiles
gMenu_AnimatedCursorTiles:
	.incbin "games/common/src/menu/cursor_spin.8bpp16x16"
