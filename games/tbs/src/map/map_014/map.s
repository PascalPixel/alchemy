	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map014
Field_Map014:
	map_header 0, 0, 112, 80, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 112, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map014, .LField_Map014_METATILES
	map_block Field_Map014, .LField_Map014_SHAPES
	map_block Field_Map014, .LField_Map014_GRID
	map_block Field_Map014, .LField_Map014_ANIMATION
	map_block Field_Map014, .LField_Map014_BLEND
	map_block Field_Map014, .LField_Map014_MARKERS
.LField_Map014_METATILES:
	.incbin "map/map_014/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map014_SHAPES:
	.incbin "map/map_014/shapes.table.lz"
	.balign 4, 0
.LField_Map014_GRID:
	.incbin "map/map_014/grid.parts.lz"
	.balign 4, 0
.LField_Map014_ANIMATION:
	.incbin "map/map_014/animation.script.lz"
	.balign 4, 0
.LField_Map014_BLEND:
	.incbin "map/map_014/blend.script.lz"
	.balign 4, 0
.LField_Map014_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
