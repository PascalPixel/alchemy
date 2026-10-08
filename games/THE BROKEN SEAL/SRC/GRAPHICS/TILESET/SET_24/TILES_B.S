	.section .rodata
	.global Tileset_Set24TilesB
Tileset_Set24TilesB:
	.incbin "GRAPHICS/TILESET/SET_24/TILES_B.4bpp.mtf"
	/* PACKER: left in the packer's buffer by TILESET/SET_24/TILES_A.o at +0x8fe */
	.byte 0x00, 0x3f
