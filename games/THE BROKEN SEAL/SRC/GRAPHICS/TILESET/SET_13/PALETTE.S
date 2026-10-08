	.section .rodata
	.global Tileset_Set13Palette
Tileset_Set13Palette:
	.incbin "GRAPHICS/TILESET/SET_13/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_015/MAP.o at +0x10a */
	.byte 0x27, 0x50
