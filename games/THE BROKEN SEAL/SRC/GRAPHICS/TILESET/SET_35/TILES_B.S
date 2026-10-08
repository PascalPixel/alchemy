	.section .rodata
	.global Tileset_Set35TilesB
Tileset_Set35TilesB:
	.incbin "GRAPHICS/TILESET/SET_35/TILES_B.4bpp.mtf"
	.byte 0x8f, 0x63	@ PACKER: left in the buffer by Tileset_Set35TilesA+0x2272
