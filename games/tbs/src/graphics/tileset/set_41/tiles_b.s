	.section .rodata
	.global Tileset_Set41TilesB
Tileset_Set41TilesB:
	.incbin "graphics/tileset/set_41/tiles_b.4bpp.mtf"
	.byte 0x69	@ PACKER: left in the buffer by Tileset_Set41TilesA+0x14b7
