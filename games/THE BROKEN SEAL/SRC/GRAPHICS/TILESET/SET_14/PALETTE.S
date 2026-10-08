	.section .rodata
	.global Tileset_Set14Palette
Tileset_Set14Palette:
	.incbin "GRAPHICS/TILESET/SET_14/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_017/MAP.o at +0x183 */
	.byte 0xc8
