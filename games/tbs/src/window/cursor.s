	.section .rodata
	.balign 4
	.global Menu_CursorLeftObjectTiles
Menu_CursorLeftObjectTiles:
	.incbin "window/cursor_l.8bpp8x16"
	.global Menu_CursorObjectTiles
Menu_CursorObjectTiles:
	.incbin "window/cursor_r.8bpp8x16"
	.global Menu_AnimatedCursorTiles
Menu_AnimatedCursorTiles:
	.incbin "window/curspin.8bpp16x16"
