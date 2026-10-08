	.section .rodata
	.global Tileset_Set50TilesB
Tileset_Set50TilesB:
	.incbin "GRAPHICS/TILESET/SET_50/TILES_B.4bpp.mtf"
	.byte 0x9c, 0x52	@ PACKER: left in the buffer by Tileset_Set50TilesA+0x220a
