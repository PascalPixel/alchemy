	.section .rodata
	.global Tileset_Set82TilesB
Tileset_Set82TilesB:
	.incbin "games/tla/src/graphics/tileset/set_82/tiles_b.4bpp.mtf"
	.byte 0x0a, 0x1c, 0x4b	@ PACKER: left in the buffer by Field_Map189+0x2d31
