	.section .rodata
	.global Tileset_Set17TilesB
Tileset_Set17TilesB:
	.incbin "games/tla/src/graphics/tileset/set_17/tiles_b.4bpp.mtf"
	.byte 0xe7, 0x01	@ PACKER: left in the buffer by Tileset_Set17TilesA+0xfda
