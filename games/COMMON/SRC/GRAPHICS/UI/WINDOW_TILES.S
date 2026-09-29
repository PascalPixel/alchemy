@ Ui_WindowTiles: 256 tiles of 4-bit colour for windows, their frames, the
@ text font and button marks, drawn with the window palette.
	.section .rodata
	.global Ui_WindowTiles
Ui_WindowTiles:
	.incbin "GRAPHICS/UI/WINDOW_TILES.4bpp"
