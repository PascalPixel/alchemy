	.section .rodata
	.balign 4
	.global gMenu_CursorLeftActorTiles
gMenu_CursorLeftActorTiles:
	.incbin "games/tbs/src/graphics/ui/sprite_cursors/left.8bpp8x16"
	.global gMenu_CursorActorTiles
gMenu_CursorActorTiles:
	.incbin "games/tbs/src/graphics/ui/sprite_cursors/right.8bpp8x16"
	.global gMenu_AnimatedCursorTiles
gMenu_AnimatedCursorTiles:
	.incbin "games/tbs/src/graphics/ui/sprite_cursors/spin.8bpp16x16"
