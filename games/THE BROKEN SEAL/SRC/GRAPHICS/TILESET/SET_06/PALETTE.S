	.section .rodata
	.global Tileset_Set06Palette
Tileset_Set06Palette:
	.incbin "GRAPHICS/TILESET/SET_06/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_007/MAP.o at +0xfd */
	.byte 0x88, 0xd7, 0x3c
