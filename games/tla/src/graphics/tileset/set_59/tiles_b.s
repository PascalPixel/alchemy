	.section .rodata
	.global Tileset_Set59TilesB
Tileset_Set59TilesB:
	.incbin "games/tla/src/graphics/tileset/set_59/tiles_b.4bpp.mtf"
	.byte 0x4d, 0xd0, 0x25	@ PACKER: left in the buffer by Tileset_Set59TilesA+0x197d
