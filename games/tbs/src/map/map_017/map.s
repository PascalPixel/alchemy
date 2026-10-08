	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map017
Field_Map017:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map017, .LField_Map017_METATILES
	map_block Field_Map017, .LField_Map017_SHAPES
	map_block Field_Map017, .LField_Map017_GRID
	map_block Field_Map017
	map_block Field_Map017
	map_block Field_Map017, .LField_Map017_MARKERS
.LField_Map017_METATILES:
	.incbin "map/map_017/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map017_SHAPES:
	.incbin "map/map_017/shapes.table.lz"
	.balign 4, 0
.LField_Map017_GRID:
	.incbin "map/map_017/grid.parts.lz"
	.balign 4, 0
.LField_Map017_MARKERS:
	.incbin "map/map_017/markers.table"
	.balign 4, 0
