	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map008
Field_Map008:
	map_header 0, 0, 30, 45, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 30, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 50, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map008, .LField_Map008_METATILES
	map_block Field_Map008, .LField_Map008_SHAPES
	map_block Field_Map008, .LField_Map008_GRID
	map_block Field_Map008
	map_block Field_Map008
	map_block Field_Map008, .LField_Map008_MARKERS
.LField_Map008_METATILES:
	.incbin "map/map_008/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map008_SHAPES:
	.incbin "map/map_008/shapes.table.lz"
	.balign 4, 0
.LField_Map008_GRID:
	.incbin "map/map_008/grid.parts.lz"
	.balign 4, 0
.LField_Map008_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
