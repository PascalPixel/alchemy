	.section .rodata
	.global Tileset_Set73TilesA
Tileset_Set73TilesA:
	.incbin "graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Map177+0x14e
