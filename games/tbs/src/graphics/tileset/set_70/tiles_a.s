	.section .rodata
	.global Tileset_Set70TilesA
Tileset_Set70TilesA:
	.incbin "graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Map174+0x14e
