	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map045
Field_Map045:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map045, .LField_Map045_METATILES
	map_block Field_Map045, .LField_Map045_SHAPES
	map_block Field_Map045, .LField_Map045_GRID
	map_block Field_Map045
	map_block Field_Map045
	map_block Field_Map045, .LField_Map045_MARKERS
.LField_Map045_METATILES:
	.incbin "map/map_045/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map045_SHAPES:
	.incbin "map/map_045/shapes.table.lz"
	.balign 4, 0
.LField_Map045_GRID:
	.incbin "map/map_045/grid.parts.lz"
	.balign 4, 0
.LField_Map045_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
