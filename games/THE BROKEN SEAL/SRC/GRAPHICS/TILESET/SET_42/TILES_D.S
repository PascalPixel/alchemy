	.section .rodata
	.global Tileset_Set42TilesD
Tileset_Set42TilesD:
	.incbin "GRAPHICS/TILESET/SET_42/TILES_D.4bpp.mtf"
	.byte 0xc4, 0x83	@ PACKER: left in the buffer by Tileset_Set42TilesC+0x156e
