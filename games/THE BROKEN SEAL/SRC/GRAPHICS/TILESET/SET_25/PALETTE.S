	.section .rodata
	.global Tileset_Set25Palette
Tileset_Set25Palette:
	.incbin "GRAPHICS/TILESET/SET_25/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_033/MAP.o at +0x15f */
	.byte 0xf3
