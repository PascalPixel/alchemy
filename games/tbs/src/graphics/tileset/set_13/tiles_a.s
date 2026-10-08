	.section .rodata
	.global Tileset_Set13TilesA
Tileset_Set13TilesA:
	.incbin "graphics/tileset/set_13/tiles_a.4bpp.mtf"
	.byte 0xfe, 0x4c	@ PACKER: left in the buffer by Field_Map015+0x1fe2
