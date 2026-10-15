	.section .rodata
	.global Tileset_Set34TilesB
Tileset_Set34TilesB:
	.incbin "games/tla/src/graphics/tileset/set_34/tiles_b.4bpp.mtf"
	.byte 0xc8, 0x4f, 0x0d	@ PACKER: left in the buffer by Tileset_Set34TilesA+0xec1
