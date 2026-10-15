	.section .rodata
	.global Tileset_Set37TilesA
Tileset_Set37TilesA:
	.incbin "games/tla/src/graphics/tileset/set_37/tiles_a.4bpp.mtf"
	.byte 0x65, 0x33	@ PACKER: left in the buffer by Field_Map073+0x1a06
