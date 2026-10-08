	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map062
Field_Map062:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map062, .LField_Map062_METATILES
	map_block Field_Map062, .LField_Map062_SHAPES
	map_block Field_Map062, .LField_Map062_GRID
	map_block Field_Map062, .LField_Map062_ANIMATION
	map_block Field_Map062, .LField_Map062_BLEND
	map_block Field_Map062, .LField_Map062_MARKERS
.LField_Map062_METATILES:
	.incbin "map/map_062/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map062_SHAPES:
	.incbin "common/map/map_062/shapes.table.lz"
	.balign 4, 0
.LField_Map062_GRID:
	.incbin "map/map_062/grid.parts.lz"
	.balign 4, 0
.LField_Map062_ANIMATION:
	.incbin "map/map_033/animation.script.lz"
	.balign 4, 0
.LField_Map062_BLEND:
	.incbin "map/map_033/blend.script.lz"
	.balign 4, 0
.LField_Map062_MARKERS:
	.incbin "map/map_062/markers.table"
	.balign 4, 0
