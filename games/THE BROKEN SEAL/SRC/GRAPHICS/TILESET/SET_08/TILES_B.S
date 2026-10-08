	.section .rodata
	.global Tileset_Set08TilesB
Tileset_Set08TilesB:
	.incbin "GRAPHICS/TILESET/SET_08/TILES_B.4bpp.mtf"
	.byte 0x18	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x1a4b
