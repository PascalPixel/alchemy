	.section .rodata
	.global Tileset_Set30Palette
Tileset_Set30Palette:
	.incbin "GRAPHICS/TILESET/SET_30/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_038/MAP.o at +0x1a1 */
	.byte 0x8e, 0x9e, 0x21
