	.section .rodata
	.balign 4
	.global gMenu_CursorLeftActorTiles
gMenu_CursorLeftActorTiles:
	.incbin "games/tbs/src/menu/cursor_l.8bpp8x16"
	.global gMenu_CursorActorTiles
gMenu_CursorActorTiles:
	.incbin "games/tbs/src/menu/cursor_r.8bpp8x16"
	.global gMenu_AnimatedCursorTiles
gMenu_AnimatedCursorTiles:
	.incbin "games/tbs/src/menu/cursor_spin.8bpp16x16"
