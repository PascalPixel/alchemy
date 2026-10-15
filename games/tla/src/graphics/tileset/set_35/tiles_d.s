	.section .rodata
	.global Tileset_Set35TilesD
Tileset_Set35TilesD:
	.incbin "games/tla/src/graphics/tileset/set_35/tiles_d.4bpp.mtf"
	.byte 0x6f, 0xd1	@ PACKER: left in the buffer by Tileset_Set35TilesB+0x1a06
