@ Four frames of the window frame's eight tiles, one frame a row, in the
@ window palette. Func_0803cbf8 copies a frame over background tiles 8 to 15
@ as a window's timer runs down (frames 3, 2, 1, 2, 3), and frame 0, the
@ frame Ui_WindowTiles draws, when it ends.
	.section .rodata
	.global Ui_WindowFrameFlash
Ui_WindowFrameFlash:
	.incbin "games/tla/src/graphics/ui/window_frame_flash.4bpp"
