	.section .rodata
	.global Tileset_Set69TilesA
Tileset_Set69TilesA:
	.incbin "graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Map173+0x14e
