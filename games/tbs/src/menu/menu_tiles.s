	.section .rodata
	.balign 4
@ 4bpp row that grows one pixel at a time in color 5
	.global Menu_PatternRowMasks
Menu_PatternRowMasks:
	.4byte 0x00055000, 0x00000000
	.4byte 0x05055050, 0x00000000
	.4byte 0x05055050, 0x00000005
	.4byte 0x05055050, 0x00000505
	.4byte 0x05055050, 0x00050505
	.4byte 0x05055050, 0x05050505
	.global Menu_ChangeArrowTile
Menu_ChangeArrowTile:
	.incbin "games/tbs/src/menu/backdrop.4bpp"
