	.section .rodata
	.global Tileset_Set40TilesA
Tileset_Set40TilesA:
	.incbin "games/tla/src/graphics/tileset/set_40/tiles_a.4bpp.mtf"
	.byte 0x09	@ PACKER: left in the buffer by Field_Map078+0x1e83
