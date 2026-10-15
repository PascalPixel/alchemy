	.section .rodata
	.global Tileset_Set121TilesB
Tileset_Set121TilesB:
	.incbin "games/tla/src/graphics/tileset/set_121/tiles_b.4bpp.mtf"
	.byte 0x0f, 0xd8	@ PACKER: left in the buffer by Tileset_Set121TilesA+0x1772
