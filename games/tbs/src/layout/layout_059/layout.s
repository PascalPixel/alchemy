	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout059
Field_Layout059:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout059, .LField_Layout059_METATILES
	layout_offset Field_Layout059, .LField_Layout059_SHAPES
	layout_offset Field_Layout059, .LField_Layout059_GRID
	layout_offset Field_Layout059, .LField_Layout059_ANIMATION
	layout_offset Field_Layout059
	layout_offset Field_Layout059, .LField_Layout059_MARKERS
.LField_Layout059_METATILES:
	.incbin "games/tbs/src/layout/layout_059/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout059_SHAPES:
	.incbin "games/tbs/src/layout/layout_059/shapes.table.lz"
	.balign 4, 0
.LField_Layout059_GRID:
	.incbin "games/tbs/src/layout/layout_059/grid.parts.lz"
	.balign 4, 0
.LField_Layout059_ANIMATION:
	.incbin "games/tbs/src/layout/layout_054/animation.script.lz"
	.balign 4, 0
.LField_Layout059_MARKERS:
	.incbin "games/tbs/src/layout/layout_059/markers.table"
	.balign 4, 0
