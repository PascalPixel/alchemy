	.section .rodata
	.global Tileset_Set52TilesA
Tileset_Set52TilesA:
	.incbin "games/tla/src/graphics/tileset/set_52/tiles_a.4bpp.mtf"
	.byte 0x5b	@ PACKER: left in the buffer by Tileset_Set51TilesA+0x28ab
