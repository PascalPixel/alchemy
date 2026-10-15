	.section .rodata
	.global Tileset_Map264TilesD
Tileset_Map264TilesD:
	.incbin "games/tla/src/graphics/tileset/map_264/tiles_d.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Map264+0xac2
