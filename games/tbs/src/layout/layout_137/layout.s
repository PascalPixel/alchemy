	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout137
Field_Layout137:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout137, .LField_Layout137_METATILES
	layout_offset Field_Layout137, .LField_Layout137_SHAPES
	layout_offset Field_Layout137, .LField_Layout137_GRID
	layout_offset Field_Layout137, .LField_Layout137_ANIMATION
	layout_offset Field_Layout137, .LField_Layout137_BLEND
	layout_offset Field_Layout137, .LField_Layout137_MARKERS
.LField_Layout137_METATILES:
	.incbin "games/tbs/src/layout/layout_137/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout137_SHAPES:
	.incbin "games/tbs/src/layout/layout_137/shapes.table.lz"
	.balign 4, 0
.LField_Layout137_GRID:
	.incbin "games/tbs/src/layout/layout_137/grid.parts.lz"
	.balign 4, 0
.LField_Layout137_ANIMATION:
	.incbin "games/tbs/src/layout/layout_022/animation.script.lz"
	.balign 4, 0
.LField_Layout137_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Layout137_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
