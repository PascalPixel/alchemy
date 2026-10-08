	.section .rodata
	.global Tileset_Set16Palette
Tileset_Set16Palette:
	.incbin "GRAPHICS/TILESET/SET_16/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_019/MAP.o at +0xf3 */
	.byte 0x3f
