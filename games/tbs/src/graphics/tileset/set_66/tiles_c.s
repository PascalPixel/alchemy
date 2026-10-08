	.section .rodata
	.global Tileset_Set66TilesC
Tileset_Set66TilesC:
	.incbin "graphics/tileset/set_66/tiles_c.4bpp.mtf"
	.byte 0xd2, 0x14	@ PACKER: left in the buffer by Tileset_Set66TilesB+0x1ef2
