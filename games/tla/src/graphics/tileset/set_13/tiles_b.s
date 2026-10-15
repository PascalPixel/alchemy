	.section .rodata
	.global Tileset_Set13TilesB
Tileset_Set13TilesB:
	.incbin "games/tla/src/graphics/tileset/set_13/tiles_b.4bpp.mtf"
	.byte 0x7f	@ PACKER: left in the buffer by Tileset_Set13TilesA+0x124b
