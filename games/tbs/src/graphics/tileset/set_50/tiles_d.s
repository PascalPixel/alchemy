	.section .rodata
	.global Tileset_Set50TilesD
Tileset_Set50TilesD:
	.incbin "graphics/tileset/set_50/tiles_d.4bpp.mtf"
	.byte 0xc0, 0x84	@ PACKER: left in the buffer by Tileset_Set50TilesB+0x1a16
