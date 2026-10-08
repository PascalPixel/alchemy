	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map036
Field_Map036:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map036, .LField_Map036_METATILES
	map_block Field_Map036, .LField_Map036_SHAPES
	map_block Field_Map036, .LField_Map036_GRID
	map_block Field_Map036, .LField_Map036_ANIMATION
	map_block Field_Map036, .LField_Map036_BLEND
	map_block Field_Map036, .LField_Map036_MARKERS
.LField_Map036_METATILES:
	.incbin "map/map_036/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map036_SHAPES:
	.incbin "map/map_036/shapes.table.lz"
	.balign 4, 0
.LField_Map036_GRID:
	.incbin "map/map_036/grid.parts.lz"
	.balign 4, 0
.LField_Map036_ANIMATION:
	.incbin "map/map_036/animation.script.lz"
	.balign 4, 0
.LField_Map036_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map036_MARKERS:
	.incbin "map/map_036/markers.table"
	.balign 4, 0
