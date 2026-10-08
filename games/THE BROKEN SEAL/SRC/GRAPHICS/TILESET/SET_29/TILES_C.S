	.section .rodata
	.global Tileset_Set29TilesC
Tileset_Set29TilesC:
	.incbin "GRAPHICS/TILESET/SET_29/TILES_C.4bpp.mtf"
	.byte 0xba	@ PACKER: left in the buffer by Tileset_Set29TilesA+0x2847
