	.section .rodata
	.global Tileset_Set21TilesA
Tileset_Set21TilesA:
	.incbin "GRAPHICS/TILESET/SET_21/TILES_A.4bpp.mtf"
	/* PACKER: left in the packer's buffer by MAP_027/MAP.o at +0x2bad */
	.byte 0xdc, 0x06, 0xaa
