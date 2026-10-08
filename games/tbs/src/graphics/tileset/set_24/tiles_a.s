	.section .rodata
	.global Tileset_Set24TilesA
Tileset_Set24TilesA:
	.incbin "graphics/tileset/set_24/tiles_a.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Field_Map032+0x16fb
