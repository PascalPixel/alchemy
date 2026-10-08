	.section .rodata
	.global Tileset_Set65TilesA
Tileset_Set65TilesA:
	.incbin "graphics/tileset/set_65/tiles_a.4bpp.mtf"
	.byte 0x94	@ PACKER: left in the buffer by Field_Map151+0x1e7f
