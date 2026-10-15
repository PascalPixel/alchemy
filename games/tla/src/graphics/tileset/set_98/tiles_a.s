	.section .rodata
	.global Tileset_Set98TilesA
Tileset_Set98TilesA:
	.incbin "games/tla/src/graphics/tileset/set_98/tiles_a.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Field_Map226+0x1b33
