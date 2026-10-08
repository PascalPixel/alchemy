	.section .rodata
	.global Tileset_Set03TilesB
Tileset_Set03TilesB:
	.incbin "graphics/tileset/set_03/tiles_b.4bpp.mtf"
	.byte 0xcb, 0x42, 0xda	@ PACKER: left in the buffer by Tileset_Set03TilesA+0x1e15
