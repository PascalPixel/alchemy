	.section .rodata
	.global Tileset_Set02Palette
Tileset_Set02Palette:
	.incbin "GRAPHICS/TILESET/SET_02/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_003/MAP.o at +0x17f */
	.byte 0xc7
