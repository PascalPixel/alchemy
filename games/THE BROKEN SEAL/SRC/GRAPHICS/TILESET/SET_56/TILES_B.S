	.section .rodata
	.global Tileset_Set56TilesB
Tileset_Set56TilesB:
	.incbin "GRAPHICS/TILESET/SET_56/TILES_B.4bpp.mtf"
	.byte 0x79, 0x3a	@ PACKER: left in the buffer by Tileset_Set54TilesB+0x20fe
