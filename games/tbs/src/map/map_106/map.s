	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map106
Field_Map106:
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map106, .LField_Map106_METATILES
	map_block Field_Map106, .LField_Map106_SHAPES
	map_block Field_Map106, .LField_Map106_GRID
	map_block Field_Map106, .LField_Map106_ANIMATION
	map_block Field_Map106
	map_block Field_Map106, .LField_Map106_MARKERS
.LField_Map106_METATILES:
	.incbin "map/map_106/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map106_SHAPES:
	.incbin "map/map_106/shapes.table.lz"
	.balign 4, 0
.LField_Map106_GRID:
	.incbin "map/map_106/grid.parts.lz"
	.balign 4, 0
.LField_Map106_ANIMATION:
	.incbin "map/map_101/animation.script.lz"
	.balign 4, 0
.LField_Map106_MARKERS:
	.incbin "map/map_106/markers.table"
