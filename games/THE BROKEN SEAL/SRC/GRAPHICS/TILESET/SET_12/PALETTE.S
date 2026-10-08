	.section .rodata
	.global Tileset_Set12Palette
Tileset_Set12Palette:
	.incbin "GRAPHICS/TILESET/SET_12/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_014/MAP.o at +0x127 */
	.byte 0xb1
