	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout156
Field_Layout156:
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout156, .LField_Layout156_METATILES
	layout_offset Field_Layout156, .LField_Layout156_SHAPES
	layout_offset Field_Layout156, .LField_Layout156_GRID
	layout_offset Field_Layout156, .LField_Layout156_ANIMATION
	layout_offset Field_Layout156, .LField_Layout156_BLEND
	layout_offset Field_Layout156, .LField_Layout156_MARKERS
.LField_Layout156_METATILES:
	.incbin "games/tbs/src/layout/layout_156/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout156_SHAPES:
	.incbin "games/tbs/src/layout/layout_156/shapes.table.lz"
	.balign 4, 0
.LField_Layout156_GRID:
	.incbin "games/tbs/src/layout/layout_156/grid.parts.lz"
	.balign 4, 0
.LField_Layout156_ANIMATION:
	.incbin "games/tbs/src/layout/layout_054/animation.script.lz"
	.balign 4, 0
.LField_Layout156_BLEND:
	.incbin "games/tbs/src/layout/layout_054/blend.script.lz"
	.balign 4, 0
.LField_Layout156_MARKERS:
	.incbin "games/tbs/src/layout/layout_156/markers.table"
	.balign 4, 0
