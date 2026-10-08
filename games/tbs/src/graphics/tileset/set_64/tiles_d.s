	.section .rodata
	.global Tileset_Set64TilesD
Tileset_Set64TilesD:
	.incbin "graphics/tileset/set_64/tiles_d.4bpp.mtf"
	.byte 0x81, 0x79	@ PACKER: left in the buffer by Tileset_Set64TilesB+0x1a26
