	.section .rodata
	.global Tileset_Set20Palette
Tileset_Set20Palette:
	.incbin "GRAPHICS/TILESET/SET_20/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_025/MAP.o at +0x19a */
	.byte 0xe2, 0x17
