	.section .rodata
	.global Tileset_Set31Palette
Tileset_Set31Palette:
	.incbin "GRAPHICS/TILESET/SET_31/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_039/MAP.o at +0x17d */
	.byte 0x9e, 0x99, 0xb7
