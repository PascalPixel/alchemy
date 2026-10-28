@ Four pictures of the window border's eight tiles, one picture a row, in the
@ window palette. Func_0803cbf8 copies a picture over background tiles 8 to 15
@ as a window's timer runs down (pictures 3, 2, 1, 2, 3), and picture 0, the
@ border Ui_WindowTiles draws, when it ends.
	.section .rodata
	.global Ui_WindowBorderFlash
Ui_WindowBorderFlash:
	.incbin "games/tla/src/graphics/ui/window_border_flash.4bpp"
