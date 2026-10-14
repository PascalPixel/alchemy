	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map012
Field_Map012:
	map_header 0, 0, 104, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 104, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map012, .LField_Map012_METATILES
	map_block Field_Map012, .LField_Map012_SHAPES
	map_block Field_Map012, .LField_Map012_GRID
	map_block Field_Map012, .LField_Map012_ANIMATION
	map_block Field_Map012, .LField_Map012_BLEND
	map_block Field_Map012, .LField_Map012_MARKERS
.LField_Map012_METATILES:
	.incbin "games/tbs/src/map/map_012/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map012_SHAPES:
	.incbin "games/common/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map012_GRID:
	.incbin "games/tbs/src/map/map_012/grid.parts.lz"
	.balign 4, 0
.LField_Map012_ANIMATION:
	.incbin "games/tbs/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map012_BLEND:
	.incbin "games/tbs/src/map/map_012/blend.script.lz"
	.balign 4, 0
.LField_Map012_MARKERS:
	.incbin "games/tbs/src/map/map_012/markers.table"
	.balign 4, 0
