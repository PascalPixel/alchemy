	.section .rodata
	.global Tileset_Set22TilesA
Tileset_Set22TilesA:
	.incbin "games/tla/src/graphics/tileset/set_22/tiles_a.4bpp.mtf"
	.byte 0x00, 0x02	@ PACKER: left in the buffer by Field_Map034+0x288a
