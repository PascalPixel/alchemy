	.section .rodata
	.global Tileset_Set117TilesB
Tileset_Set117TilesB:
	.incbin "games/tla/src/graphics/tileset/set_117/tiles_b.4bpp.mtf"
	.byte 0xe0, 0x28	@ PACKER: left in the buffer by Tileset_Set117TilesA+0xdf6
