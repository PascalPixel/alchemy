	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map004
Field_Map004:
	map_header 0, 0, 60, 70, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map004, .LField_Map004_CELL_PICTURES
	map_offset Field_Map004, .LField_Map004_SHAPES
	map_offset Field_Map004, .LField_Map004_GRID
	map_offset Field_Map004
	map_offset Field_Map004
	map_offset Field_Map004, .LField_Map004_MARKERS
.LField_Map004_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_004/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map004_SHAPES:
	.incbin "games/tbs/src/map/map_004/shapes.table.lz"
	.balign 4, 0
.LField_Map004_GRID:
	.incbin "games/tbs/src/map/map_004/map.grid.lz"
	.balign 4, 0
.LField_Map004_MARKERS:
	treasure_spot 8, 12, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set03Palette
Tileset_Set03Palette:
	.incbin "games/tbs/src/graphics/tileset/set_03/palette.gbapal.lz"
	.byte 0xc1, 0xac, 0xce	@ PACKER: left in the buffer by Field_Map004+0x191
	.global Tileset_Set03TilesA
Tileset_Set03TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_03/tiles_a.4bpp.mtf"
	.byte 0x1e	@ PACKER: left in the buffer by Field_Map003+0x283f
	.global Tileset_Set03TilesB
Tileset_Set03TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_03/tiles_b.4bpp.mtf"
	.byte 0xcb, 0x42, 0xda	@ PACKER: left in the buffer by Tileset_Set03TilesA+0x1e15
	.global Tileset_Set03TilesC
Tileset_Set03TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_03/tiles_c.4bpp.mtf"
	.byte 0xeb, 0x91	@ PACKER: left in the buffer by Tileset_Set03TilesA+0x26de
	.global Tileset_Set03TilesD
Tileset_Set03TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_03/tiles_d.4bpp.mtf"
	.byte 0xe1, 0xf0, 0xf8	@ PACKER: left in the buffer by Tileset_Set03TilesC+0x1145
