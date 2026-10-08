	.section .rodata
	.global Tileset_Set62TilesB
Tileset_Set62TilesB:
	.incbin "graphics/tileset/set_62/tiles_b.4bpp.mtf"
	.byte 0xba	@ PACKER: left in the buffer by Tileset_Set62TilesA+0x1c43
