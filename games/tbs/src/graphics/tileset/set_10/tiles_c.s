	.section .rodata
	.global Tileset_Set10TilesC
Tileset_Set10TilesC:
	.incbin "graphics/tileset/set_10/tiles_c.4bpp.mtf"
	.byte 0x12, 0xa2	@ PACKER: left in the buffer by Tileset_Set10TilesA+0xc4a
