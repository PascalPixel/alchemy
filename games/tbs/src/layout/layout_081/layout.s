	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout081
Field_Layout081:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout081, .LField_Layout081_METATILES
	layout_offset Field_Layout081, .LField_Layout081_SHAPES
	layout_offset Field_Layout081, .LField_Layout081_GRID
	layout_offset Field_Layout081, .LField_Layout081_ANIMATION
	layout_offset Field_Layout081, .LField_Layout081_BLEND
	layout_offset Field_Layout081, .LField_Layout081_MARKERS
.LField_Layout081_METATILES:
	.incbin "games/tbs/src/layout/layout_081/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout081_SHAPES:
	.incbin "games/tbs/src/layout/layout_081/shapes.table.lz"
	.balign 4, 0
.LField_Layout081_GRID:
	.incbin "games/tbs/src/layout/layout_081/grid.parts.lz"
	.balign 4, 0
.LField_Layout081_ANIMATION:
	.incbin "games/tbs/src/layout/layout_078/animation.script.lz"
	.balign 4, 0
.LField_Layout081_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout081_MARKERS:
	.incbin "games/tbs/src/layout/layout_081/markers.table"
	.balign 4, 0
