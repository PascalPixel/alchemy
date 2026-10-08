	.section .rodata
	.global Tileset_Set18Palette
Tileset_Set18Palette:
	.incbin "GRAPHICS/TILESET/SET_18/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_021/MAP.o at +0xcb */
	.byte 0x71
