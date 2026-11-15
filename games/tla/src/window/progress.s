@ The save screen's progress bar's eight tiles; gWindow_ProgressBarRows
@ (screen_tables.c) lays them out by step.
	.section .rodata
	.global gWindow_ProgressBarTiles
gWindow_ProgressBarTiles:
	.incbin "games/tla/src/window/progress.4bpp.lz"
