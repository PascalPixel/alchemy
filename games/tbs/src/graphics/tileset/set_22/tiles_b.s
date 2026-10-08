	.section .rodata
	.global Tileset_Set22TilesB
Tileset_Set22TilesB:
	.incbin "graphics/tileset/set_22/tiles_b.4bpp.mtf"
	.byte 0xf9	@ PACKER: left in the buffer by Tileset_Set22TilesA+0x1dcf
