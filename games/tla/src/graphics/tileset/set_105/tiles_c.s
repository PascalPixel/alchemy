	.section .rodata
	.global Tileset_Set105TilesC
Tileset_Set105TilesC:
	.incbin "games/tla/src/graphics/tileset/set_105/tiles_c.4bpp.mtf"
	.byte 0xf2, 0x00, 0x7f	@ PACKER: left in the buffer by Tileset_Set105TilesB+0x871
