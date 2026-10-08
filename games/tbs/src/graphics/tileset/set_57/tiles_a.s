	.section .rodata
	.global Tileset_Set57TilesA
Tileset_Set57TilesA:
	.incbin "graphics/tileset/set_57/tiles_a.4bpp.mtf"
	.byte 0xc0, 0xbe, 0xc0	@ PACKER: left in the buffer by Tileset_Set56TilesC+0x13f1
