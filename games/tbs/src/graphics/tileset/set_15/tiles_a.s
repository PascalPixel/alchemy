	.section .rodata
	.global Tileset_Set15TilesA
Tileset_Set15TilesA:
	.incbin "graphics/tileset/set_15/tiles_a.4bpp.mtf"
	.byte 0x80, 0x36, 0x05	@ PACKER: left in the buffer by Field_Map018+0x1c29
