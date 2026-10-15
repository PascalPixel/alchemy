	.section .rodata
	.global Tileset_Set61TilesA
Tileset_Set61TilesA:
	.incbin "games/tla/src/graphics/tileset/set_61/tiles_a.4bpp.mtf"
	.byte 0x4d, 0x25	@ PACKER: left in the buffer by Tileset_Set59TilesA+0x2a36
