	.section .rodata
	.global Tileset_Set00Palette
Tileset_Set00Palette:
	.incbin "GRAPHICS/TILESET/SET_00/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_000/MAP.o at +0x18d */
	.byte 0x3f, 0x55, 0x78
