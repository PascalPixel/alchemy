	.section .rodata
	.global Tileset_Set59TilesB
Tileset_Set59TilesB:
	.incbin "graphics/tileset/set_59/tiles_b.4bpp.mtf"
	.byte 0x8c, 0xba	@ PACKER: left in the buffer by Tileset_Set59TilesA+0x1c3e
