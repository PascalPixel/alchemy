	.section .rodata
	.global Tileset_Set14TilesA
Tileset_Set14TilesA:
	.incbin "graphics/tileset/set_14/tiles_a.4bpp.mtf"
	.byte 0x8a, 0x33, 0x2a	@ PACKER: left in the buffer by Field_Map006+0x2b51
