	.section .rodata
	.global Tileset_Set50TilesC
Tileset_Set50TilesC:
	.incbin "graphics/tileset/set_50/tiles_c.4bpp.mtf"
	.byte 0x96	@ PACKER: left in the buffer by Tileset_Set50TilesB+0x13f
