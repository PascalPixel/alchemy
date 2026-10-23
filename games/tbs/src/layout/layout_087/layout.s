	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout087
Field_Layout087:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout087, .LField_Layout087_METATILES
	layout_offset Field_Layout087, .LField_Layout087_SHAPES
	layout_offset Field_Layout087, .LField_Layout087_GRID
	layout_offset Field_Layout087, .LField_Layout087_ANIMATION
	layout_offset Field_Layout087, .LField_Layout087_BLEND
	layout_offset Field_Layout087, .LField_Layout087_MARKERS
.LField_Layout087_METATILES:
	.incbin "games/tbs/src/layout/layout_087/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout087_SHAPES:
	.incbin "games/tbs/src/layout/layout_087/shapes.table.lz"
	.balign 4, 0
.LField_Layout087_GRID:
	.incbin "games/tbs/src/layout/layout_087/grid.parts.lz"
	.balign 4, 0
.LField_Layout087_ANIMATION:
	.incbin "games/tbs/src/layout/layout_080/animation.script.lz"
	.balign 4, 0
.LField_Layout087_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout087_MARKERS:
	.incbin "games/tbs/src/layout/layout_087/markers.table"
	.balign 4, 0
