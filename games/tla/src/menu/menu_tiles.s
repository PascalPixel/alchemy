	.section .rodata
	.balign 4
@ 4bpp rows of dots in colour 5, each reaching further right than the last
	.global MenuPatternRowMasks
MenuPatternRowMasks:
	.4byte 0x00055000, 0x00000000
	.4byte 0x05055050, 0x00000000
	.4byte 0x05055050, 0x00000005
	.4byte 0x05055050, 0x00000505
	.4byte 0x05055050, 0x00050505
	.4byte 0x05055050, 0x05050505
	.global Menu_BackdropBorderTile
Menu_BackdropBorderTile:
	.incbin "games/tla/src/menu/class_change_arrow.4bpp"
