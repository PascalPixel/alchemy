	.section .rodata
	.global Tileset_Set19TilesB
Tileset_Set19TilesB:
	.incbin "GRAPHICS/TILESET/SET_19/TILES_B.4bpp.mtf"
	.byte 0x27, 0x88	@ PACKER: left in the buffer by Tileset_Set19TilesA+0x20fa
