	.section .rodata
	.global Tileset_Set35TilesA
Tileset_Set35TilesA:
	.incbin "games/tla/src/graphics/tileset/set_35/tiles_a.4bpp.mtf"
	.byte 0xe6, 0x08	@ PACKER: left in the buffer by Tileset_Set29TilesA+0x2ab6
