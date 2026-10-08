	.section .rodata
	.global Tileset_Set12TilesC
Tileset_Set12TilesC:
	.incbin "graphics/tileset/set_12/tiles_c.4bpp.mtf"
	.byte 0x8a, 0x18, 0x32	@ PACKER: left in the buffer by Tileset_Set12TilesB+0xf3d
