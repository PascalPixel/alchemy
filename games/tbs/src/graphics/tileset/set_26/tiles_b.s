	.section .rodata
	.global Tileset_Set26TilesB
Tileset_Set26TilesB:
	.incbin "graphics/tileset/set_26/tiles_b.4bpp.mtf"
	.byte 0xf1, 0xb3, 0x21	@ PACKER: left in the buffer by Tileset_Set26TilesA+0x2779
