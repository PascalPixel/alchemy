	.section .rodata
	.global Tileset_Set30TilesB
Tileset_Set30TilesB:
	.incbin "games/tla/src/graphics/tileset/set_30/tiles_b.4bpp.mtf"
	.byte 0x5a, 0x14	@ PACKER: left in the buffer by Tileset_Set29TilesC+0x2336
