	.section .rodata
	.global Tileset_Set54TilesB
Tileset_Set54TilesB:
	.incbin "graphics/tileset/set_54/tiles_b.4bpp.mtf"
	.byte 0x83	@ PACKER: left in the buffer by Tileset_Set54TilesA+0x23e3
