	.section .rodata
	.global Tileset_Set10TilesC
Tileset_Set10TilesC:
	.incbin "GRAPHICS/TILESET/SET_10/TILES_C.4bpp.mtf"
	.byte 0x12, 0xa2	@ PACKER: left in the buffer by Tileset_Set10TilesA+0xc4a
