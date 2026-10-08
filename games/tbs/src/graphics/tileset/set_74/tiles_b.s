	.section .rodata
	.global Tileset_Set74TilesB
Tileset_Set74TilesB:
	.incbin "graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set74TilesA+0x13f
