	.section .rodata
	.global Tileset_Palette00
Tileset_Palette00:
	.incbin "GRAPHICS/TILESET/PALETTES/PAL_00.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_016/MAP.o at +0x159 */
	.byte 0x41, 0x28, 0x40
