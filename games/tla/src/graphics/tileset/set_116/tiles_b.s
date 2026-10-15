	.section .rodata
	.global Tileset_Set116TilesB
Tileset_Set116TilesB:
	.incbin "games/tla/src/graphics/tileset/set_116/tiles_b.4bpp.mtf"
	.byte 0xc2, 0x04, 0x38	@ PACKER: left in the buffer by Tileset_Set116TilesA+0x126d
