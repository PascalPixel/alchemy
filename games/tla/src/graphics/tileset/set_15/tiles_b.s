	.section .rodata
	.global Tileset_Set15TilesB
Tileset_Set15TilesB:
	.incbin "games/tla/src/graphics/tileset/set_15/tiles_b.4bpp.mtf"
	.byte 0xe7, 0x01	@ PACKER: left in the buffer by Tileset_Set15TilesA+0xfda
