	.section .rodata
@ Three horizontally scrolling rows, one per 32x8 sprite
	.global gSolStatue_SpriteRowsGfx
gSolStatue_SpriteRowsGfx:
	.incbin "games/tbs/src/graphics/sol_statue/rows.4bpp32x8.plz"
