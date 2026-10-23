	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout056
Field_Layout056:
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout056, .LField_Layout056_METATILES
	layout_offset Field_Layout056, .LField_Layout056_SHAPES
	layout_offset Field_Layout056, .LField_Layout056_GRID
	layout_offset Field_Layout056, .LField_Layout056_ANIMATION
	layout_offset Field_Layout056, .LField_Layout056_BLEND
	layout_offset Field_Layout056, .LField_Layout056_MARKERS
.LField_Layout056_METATILES:
	.incbin "games/tbs/src/layout/layout_056/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout056_SHAPES:
	.incbin "games/tbs/src/layout/layout_056/shapes.table.lz"
	.balign 4, 0
.LField_Layout056_GRID:
	.incbin "games/tbs/src/layout/layout_056/grid.parts.lz"
	.balign 4, 0
.LField_Layout056_ANIMATION:
	.incbin "games/tbs/src/layout/layout_054/animation.script.lz"
	.balign 4, 0
.LField_Layout056_BLEND:
	.incbin "games/tbs/src/layout/layout_054/blend.script.lz"
	.balign 4, 0
.LField_Layout056_MARKERS:
	.incbin "games/tbs/src/layout/layout_056/markers.table"
