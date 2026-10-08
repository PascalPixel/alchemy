	.section .rodata
	.global Tileset_Set57TilesC
Tileset_Set57TilesC:
	.incbin "graphics/tileset/set_57/tiles_c.4bpp.mtf"
	.byte 0xd6, 0x83, 0x39	@ PACKER: left in the buffer by Tileset_Set57TilesB+0x1271
