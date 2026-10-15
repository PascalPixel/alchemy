	.section .rodata
	.global Tileset_Set45TilesB
Tileset_Set45TilesB:
	.incbin "games/tla/src/graphics/tileset/set_45/tiles_b.4bpp.mtf"
	.byte 0x8b, 0x70	@ PACKER: left in the buffer by Tileset_Set45TilesA+0x104a
