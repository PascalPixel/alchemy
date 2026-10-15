	.section .rodata
	.global Tileset_Set06TilesB
Tileset_Set06TilesB:
	.incbin "games/tla/src/graphics/tileset/set_06/tiles_b.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Tileset_Set06TilesA+0x270b
