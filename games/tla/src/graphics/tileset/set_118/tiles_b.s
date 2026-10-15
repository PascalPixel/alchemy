	.section .rodata
	.global Tileset_Set118TilesB
Tileset_Set118TilesB:
	.incbin "games/tla/src/graphics/tileset/set_118/tiles_b.4bpp.mtf"
	.byte 0x0c, 0xea, 0x4c	@ PACKER: left in the buffer by Tileset_Set118TilesA+0x115d
