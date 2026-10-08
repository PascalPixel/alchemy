	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map125
Field_Map125:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map125, .LField_Map125_METATILES
	map_block Field_Map125, .LField_Map125_SHAPES
	map_block Field_Map125, .LField_Map125_GRID
	map_block Field_Map125, .LField_Map125_ANIMATION
	map_block Field_Map125, .LField_Map125_BLEND
	map_block Field_Map125, .LField_Map125_MARKERS
.LField_Map125_METATILES:
	.incbin "map/map_125/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map125_SHAPES:
	.incbin "map/map_125/shapes.table.lz"
	.balign 4, 0
.LField_Map125_GRID:
	.incbin "map/map_125/grid.parts.lz"
	.balign 4, 0
.LField_Map125_ANIMATION:
	.incbin "map/map_125/animation.script.lz"
	.balign 4, 0
.LField_Map125_BLEND:
	.incbin "map/map_125/blend.script.lz"
	.balign 4, 0
.LField_Map125_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
