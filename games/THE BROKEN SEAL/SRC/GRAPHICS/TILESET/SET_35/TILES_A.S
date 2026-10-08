	.section .rodata
	.global Tileset_Set35TilesA
Tileset_Set35TilesA:
	.incbin "GRAPHICS/TILESET/SET_35/TILES_A.4bpp.mtf"
	.byte 0x07, 0xf0	@ PACKER: left in the buffer by Tileset_Set34TilesA+0x2bae
