	.section .rodata
	.global Tileset_Set62TilesD
Tileset_Set62TilesD:
	.incbin "graphics/tileset/set_62/tiles_d.4bpp.mtf"
	.byte 0xa3	@ PACKER: left in the buffer by Tileset_Set62TilesB+0x1b7b
