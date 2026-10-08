	.section .rodata
	.global Tileset_Set15TilesC
Tileset_Set15TilesC:
	.incbin "graphics/tileset/set_15/tiles_c.4bpp.mtf"
	.byte 0x00, 0xa5	@ PACKER: left in the buffer by Tileset_Set15TilesB+0x12ca
