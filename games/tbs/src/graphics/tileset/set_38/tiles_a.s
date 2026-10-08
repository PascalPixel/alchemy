	.section .rodata
	.global Tileset_Set38TilesA
Tileset_Set38TilesA:
	.incbin "graphics/tileset/set_38/tiles_a.4bpp.mtf"
	.byte 0x80, 0x03	@ PACKER: left in the buffer by Field_Map048+0x13da
