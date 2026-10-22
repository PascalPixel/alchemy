	.section .rodata
@ Three horizontally scrolling rows, one per 32x8 sprite
	.global gSolSanctumStatue_SpriteRowsGraphics
gSolSanctumStatue_SpriteRowsGraphics:
	.incbin "games/tbs/src/graphics/sol_sanctum_statue/rows.4bpp32x8.plz"
