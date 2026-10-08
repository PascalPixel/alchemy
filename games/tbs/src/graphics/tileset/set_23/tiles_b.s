	.section .rodata
	.global Tileset_Set23TilesB
Tileset_Set23TilesB:
	.incbin "graphics/tileset/set_23/tiles_b.4bpp.mtf"
	.byte 0xa6, 0x09, 0x9b	@ PACKER: left in the buffer by Tileset_Set23TilesA+0x28c5
