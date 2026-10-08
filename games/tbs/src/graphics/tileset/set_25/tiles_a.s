	.section .rodata
	.global Tileset_Set25TilesA
Tileset_Set25TilesA:
	.incbin "graphics/tileset/set_25/tiles_a.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Field_Map032+0x1c57
