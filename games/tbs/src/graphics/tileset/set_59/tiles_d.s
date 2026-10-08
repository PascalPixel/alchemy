	.section .rodata
	.global Tileset_Set59TilesD
Tileset_Set59TilesD:
	.incbin "graphics/tileset/set_59/tiles_d.4bpp.mtf"
	.byte 0x58	@ PACKER: left in the buffer by Tileset_Set59TilesB+0x1377
