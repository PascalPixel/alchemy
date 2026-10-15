	.section .rodata
	.global Tileset_Set97TilesB
Tileset_Set97TilesB:
	.incbin "games/tla/src/graphics/tileset/set_97/tiles_b.4bpp.mtf"
	.byte 0xf3, 0x7d	@ PACKER: left in the buffer by Tileset_Set97TilesA+0x199a
