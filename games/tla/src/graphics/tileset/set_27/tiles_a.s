	.section .rodata
	.global Tileset_Set27TilesA
Tileset_Set27TilesA:
	.incbin "games/tla/src/graphics/tileset/set_27/tiles_a.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Field_Map041+0x22d3
