	.section .rodata
	.global Tileset_Set10TilesB
Tileset_Set10TilesB:
	.incbin "graphics/tileset/set_10/tiles_b.4bpp.mtf"
	.byte 0x7d, 0x04, 0xc1	@ PACKER: left in the buffer by Tileset_Set10TilesA+0x945
