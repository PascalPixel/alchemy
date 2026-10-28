	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map071
Field_Map071:
	map_header 0, 0, 108, 90, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map071, .LField_Map071_CELL_PICTURES
	map_offset Field_Map071, .LField_Map071_SHAPES
	map_offset Field_Map071, .LField_Map071_GRID
	map_offset Field_Map071
	map_offset Field_Map071
	map_offset Field_Map071, .LField_Map071_MARKERS
.LField_Map071_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_071/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map071_SHAPES:
	.incbin "games/tbs/src/map/map_042/shapes.table.lz"
	.balign 4, 0
.LField_Map071_GRID:
	.incbin "games/tbs/src/map/map_071/map.grid.lz"
	.balign 4, 0
.LField_Map071_MARKERS:
	treasure_spot 8, 34, 100
	treasure_spots_end
	.balign 4, 0
