	.section .rodata
	.global Tileset_Set06TilesC
Tileset_Set06TilesC:
	.incbin "graphics/tileset/set_06/tiles_c.4bpp.mtf"
	.byte 0x50, 0xa3	@ PACKER: left in the buffer by Tileset_Set06TilesB+0x986
