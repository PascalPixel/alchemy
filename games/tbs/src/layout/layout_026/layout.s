	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout026
Field_Layout026:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout026, .LField_Layout026_METATILES
	layout_offset Field_Layout026, .LField_Layout026_SHAPES
	layout_offset Field_Layout026, .LField_Layout026_GRID
	layout_offset Field_Layout026, .LField_Layout026_ANIMATION
	layout_offset Field_Layout026, .LField_Layout026_BLEND
	layout_offset Field_Layout026, .LField_Layout026_MARKERS
.LField_Layout026_METATILES:
	.incbin "games/tbs/src/layout/layout_026/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout026_SHAPES:
	.incbin "games/tbs/src/layout/layout_026/shapes.table.lz"
	.balign 4, 0
.LField_Layout026_GRID:
	.incbin "games/tbs/src/layout/layout_026/grid.parts.lz"
	.balign 4, 0
.LField_Layout026_ANIMATION:
	.incbin "games/tbs/src/layout/layout_025/animation.script.lz"
	.balign 4, 0
.LField_Layout026_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout026_MARKERS:
	.incbin "games/tbs/src/layout/layout_026/markers.table"
	.balign 4, 0
