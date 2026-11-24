	.section .rodata
@ 4bpp rows of dots in colour 5, each reaching further right than the last
	.global gMenu_PatternRowMasks
gMenu_PatternRowMasks:
	.4byte 0x00055000, 0x00000000
	.4byte 0x05055050, 0x00000000
	.4byte 0x05055050, 0x00000005
	.4byte 0x05055050, 0x00000505
	.4byte 0x05055050, 0x00050505
	.4byte 0x05055050, 0x05050505
	.global gMenu_BackdropBorderTile
gMenu_BackdropBorderTile:
	.incbin "games/tla/src/menu/class_change_arrow.4bpp"
