	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map054
Field_Map054:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map054, .LField_Map054_METATILES
	map_block Field_Map054, .LField_Map054_SHAPES
	map_block Field_Map054, .LField_Map054_GRID
	map_block Field_Map054, .LField_Map054_ANIMATION
	map_block Field_Map054, .LField_Map054_BLEND
	map_block Field_Map054, .LField_Map054_MARKERS
.LField_Map054_METATILES:
	.incbin "map/map_054/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map054_SHAPES:
	.incbin "map/map_054/shapes.table.lz"
	.balign 4, 0
.LField_Map054_GRID:
	.incbin "map/map_054/grid.parts.lz"
	.balign 4, 0
.LField_Map054_ANIMATION:
	.incbin "map/map_054/animation.script.lz"
	.balign 4, 0
.LField_Map054_BLEND:
	.incbin "common/map/map_054/blend.script.lz"
	.balign 4, 0
.LField_Map054_MARKERS:
	.incbin "map/map_054/markers.table"
	.balign 4, 0
