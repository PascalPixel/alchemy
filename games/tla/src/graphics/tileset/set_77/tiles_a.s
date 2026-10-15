	.section .rodata
	.global Tileset_Set77TilesA
Tileset_Set77TilesA:
	.incbin "games/tla/src/graphics/tileset/set_77/tiles_a.4bpp.mtf"
	.byte 0x20	@ PACKER: left in the buffer by Field_Map167+0x1c6b
