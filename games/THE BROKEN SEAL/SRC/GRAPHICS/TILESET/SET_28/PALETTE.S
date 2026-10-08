	.section .rodata
	.global Tileset_Set28Palette
Tileset_Set28Palette:
	.incbin "GRAPHICS/TILESET/SET_28/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_036/MAP.o at +0x1a1 */
	.byte 0x40, 0x15, 0x08
