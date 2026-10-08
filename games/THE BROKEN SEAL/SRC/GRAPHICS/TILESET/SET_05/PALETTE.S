	.section .rodata
	.global Tileset_Set05Palette
Tileset_Set05Palette:
	.incbin "GRAPHICS/TILESET/SET_05/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_006/MAP.o at +0x18d */
	.byte 0x64, 0x36, 0x80
