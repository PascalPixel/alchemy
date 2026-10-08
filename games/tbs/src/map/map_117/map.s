	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map117
Field_Map117:
	map_header 0, 0, 45, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map117, .LField_Map117_METATILES
	map_block Field_Map117, .LField_Map117_SHAPES
	map_block Field_Map117, .LField_Map117_GRID
	map_block Field_Map117, .LField_Map117_ANIMATION
	map_block Field_Map117
	map_block Field_Map117, .LField_Map117_MARKERS
.LField_Map117_METATILES:
	.incbin "map/map_117/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map117_SHAPES:
	.incbin "map/map_117/shapes.table.lz"
	.balign 4, 0
.LField_Map117_GRID:
	.incbin "map/map_117/grid.parts.lz"
	.balign 4, 0
.LField_Map117_ANIMATION:
	.incbin "map/map_103/animation.script.lz"
	.balign 4, 0
.LField_Map117_MARKERS:
	.incbin "map/map_117/markers.table"
	.balign 4, 0
