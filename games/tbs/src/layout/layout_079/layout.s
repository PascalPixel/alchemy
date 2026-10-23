	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map079
Field_Map079:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map079, .LField_Map079_METATILES
	map_block Field_Map079, .LField_Map079_SHAPES
	map_block Field_Map079, .LField_Map079_GRID
	map_block Field_Map079, .LField_Map079_ANIMATION
	map_block Field_Map079, .LField_Map079_BLEND
	map_block Field_Map079, .LField_Map079_MARKERS
.LField_Map079_METATILES:
	.incbin "games/tbs/src/layout/layout_079/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map079_SHAPES:
	.incbin "games/tbs/src/layout/layout_079/shapes.table.lz"
	.balign 4, 0
.LField_Map079_GRID:
	.incbin "games/tbs/src/layout/layout_079/grid.parts.lz"
	.balign 4, 0
.LField_Map079_ANIMATION:
	.incbin "games/tbs/src/layout/layout_078/animation.script.lz"
	.balign 4, 0
.LField_Map079_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Map079_MARKERS:
	.incbin "games/tbs/src/layout/layout_079/markers.table"
	.balign 4, 0
