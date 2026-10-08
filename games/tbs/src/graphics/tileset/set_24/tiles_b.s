	.section .rodata
	.global Tileset_Set24TilesB
Tileset_Set24TilesB:
	.incbin "graphics/tileset/set_24/tiles_b.4bpp.mtf"
	.byte 0x00, 0x3f	@ PACKER: left in the buffer by Tileset_Set24TilesA+0x8fe
