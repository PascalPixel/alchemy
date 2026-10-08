	.section .rodata
	.global Tileset_Set24TilesB
Tileset_Set24TilesB:
	.incbin "GRAPHICS/TILESET/SET_24/TILES_B.4bpp.mtf"
	.byte 0x00, 0x3f	@ PACKER: left in the buffer by Tileset_Set24TilesA+0x8fe
