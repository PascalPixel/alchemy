	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map101
Field_Map101:
	map_header 5, 124, 51, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map101, .LField_Map101_METATILES
	map_block Field_Map101, .LField_Map101_SHAPES
	map_block Field_Map101, .LField_Map101_GRID
	map_block Field_Map101, .LField_Map101_ANIMATION
	map_block Field_Map101
	map_block Field_Map101, .LField_Map101_MARKERS
.LField_Map101_METATILES:
	.incbin "map/map_101/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map101_SHAPES:
	.incbin "map/map_101/shapes.table.lz"
	.balign 4, 0
.LField_Map101_GRID:
	.incbin "map/map_101/grid.parts.lz"
	.balign 4, 0
.LField_Map101_ANIMATION:
	.incbin "map/map_101/animation.script.lz"
	.balign 4, 0
.LField_Map101_MARKERS:
	.incbin "map/map_101/markers.table"
	.balign 4, 0
