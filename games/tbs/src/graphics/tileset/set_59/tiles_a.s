	.section .rodata
	.global Tileset_Set59TilesA
Tileset_Set59TilesA:
	.incbin "graphics/tileset/set_59/tiles_a.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Field_Map123+0x2a13
