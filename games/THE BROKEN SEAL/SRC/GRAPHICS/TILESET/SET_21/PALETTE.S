	.section .rodata
	.global Tileset_Set21Palette
Tileset_Set21Palette:
	.incbin "GRAPHICS/TILESET/SET_21/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_028/MAP.o at +0x19a */
	.byte 0x98, 0x49
