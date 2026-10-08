	.section .rodata
	.global Tileset_Set50TilesA
Tileset_Set50TilesA:
	.incbin "graphics/tileset/set_50/tiles_a.4bpp.mtf"
	.byte 0x54	@ PACKER: left in the buffer by Field_Map089+0x28ef
