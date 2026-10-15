	.section .rodata
	.global Tileset_Set48TilesB
Tileset_Set48TilesB:
	.incbin "games/tla/src/graphics/tileset/set_48/tiles_b.4bpp.mtf"
	.byte 0xd6	@ PACKER: left in the buffer by Tileset_Set48TilesA+0x13cf
