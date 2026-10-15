	.section .rodata
	.global Tileset_Set23TilesB
Tileset_Set23TilesB:
	.incbin "games/tla/src/graphics/tileset/set_23/tiles_b.4bpp.mtf"
	.byte 0x62, 0xf8, 0x54	@ PACKER: left in the buffer by Field_Map040+0x1e1d
