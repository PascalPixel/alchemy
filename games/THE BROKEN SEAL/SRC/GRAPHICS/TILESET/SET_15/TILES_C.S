	.section .rodata
	.global Tileset_Set15TilesC
Tileset_Set15TilesC:
	.incbin "GRAPHICS/TILESET/SET_15/TILES_C.4bpp.mtf"
	/* PACKER: left in the packer's buffer by TILESET/SET_15/TILES_B.o at +0x12ca */
	.byte 0x00, 0xa5
