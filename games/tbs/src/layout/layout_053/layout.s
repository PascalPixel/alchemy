	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout053
Field_Layout053:
	layout_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 70, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout053, .LField_Layout053_METATILES
	layout_offset Field_Layout053, .LField_Layout053_SHAPES
	layout_offset Field_Layout053, .LField_Layout053_GRID
	layout_offset Field_Layout053, .LField_Layout053_ANIMATION
	layout_offset Field_Layout053, .LField_Layout053_BLEND
	layout_offset Field_Layout053, .LField_Layout053_MARKERS
.LField_Layout053_METATILES:
	.incbin "games/tbs/src/layout/layout_053/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout053_SHAPES:
	.incbin "games/tbs/src/layout/layout_001/shapes.table.lz"
	.balign 4, 0
.LField_Layout053_GRID:
	.incbin "games/tbs/src/layout/layout_053/grid.parts.lz"
	.balign 4, 0
.LField_Layout053_ANIMATION:
	.incbin "games/tbs/src/layout/layout_053/animation.script.lz"
	.balign 4, 0
.LField_Layout053_BLEND:
	.incbin "games/tbs/src/layout/layout_053/blend.script.lz"
	.balign 4, 0
.LField_Layout053_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
