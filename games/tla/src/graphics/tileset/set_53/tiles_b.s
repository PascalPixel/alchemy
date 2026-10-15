	.section .rodata
	.global Tileset_Set53TilesB
Tileset_Set53TilesB:
	.incbin "games/tla/src/graphics/tileset/set_53/tiles_b.4bpp.mtf"
	.byte 0x0f, 0xbc, 0x0e	@ PACKER: left in the buffer by Tileset_Set53TilesA+0x20e5
