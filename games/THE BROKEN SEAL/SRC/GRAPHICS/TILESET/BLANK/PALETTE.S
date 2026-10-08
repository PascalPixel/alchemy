	.section .rodata
	.global Tileset_BlankPalette
Tileset_BlankPalette:
	.incbin "GRAPHICS/TILESET/BLANK/PALETTE.gbapal.lz"
	/* PACKER: left in the packer's buffer by MAP_001/MAP.o at +0xb */
	.byte 0x00
