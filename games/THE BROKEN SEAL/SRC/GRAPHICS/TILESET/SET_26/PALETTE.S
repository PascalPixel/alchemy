	.section .rodata
	.global Tileset_Set26Palette
Tileset_Set26Palette:
	.incbin "GRAPHICS/TILESET/SET_26/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_034/MAP.o at +0x183 */
	.byte 0x08
