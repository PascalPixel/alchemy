	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout030
Field_Layout030:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout030, .LField_Layout030_METATILES
	layout_offset Field_Layout030, .LField_Layout030_SHAPES
	layout_offset Field_Layout030, .LField_Layout030_GRID
	layout_offset Field_Layout030, .LField_Layout030_ANIMATION
	layout_offset Field_Layout030
	layout_offset Field_Layout030, .LField_Layout030_MARKERS
.LField_Layout030_METATILES:
	.incbin "games/tbs/src/layout/layout_030/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout030_SHAPES:
	.incbin "games/tbs/src/layout/layout_030/shapes.table.lz"
	.balign 4, 0
.LField_Layout030_GRID:
	.incbin "games/tbs/src/layout/layout_030/grid.parts.lz"
	.balign 4, 0
.LField_Layout030_ANIMATION:
	.incbin "games/tbs/src/layout/layout_029/animation.script.lz"
	.balign 4, 0
.LField_Layout030_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
