	.section .rodata
	.global Tileset_Set61TilesA
Tileset_Set61TilesA:
	.incbin "graphics/tileset/set_61/tiles_a.4bpp.mtf"
	.byte 0x91	@ PACKER: left in the buffer by Tileset_Set60TilesA+0x2907
