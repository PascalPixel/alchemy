	.section .rodata
	.global Tileset_Set21TilesB
Tileset_Set21TilesB:
	.incbin "graphics/tileset/set_21/tiles_b.4bpp.mtf"
	.byte 0xf1, 0xe8, 0x05	@ PACKER: left in the buffer by Tileset_Set21TilesA+0x20d1
