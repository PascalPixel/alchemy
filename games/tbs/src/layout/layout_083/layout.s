	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout083
Field_Layout083:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout083, .LField_Layout083_METATILES
	layout_offset Field_Layout083, .LField_Layout083_SHAPES
	layout_offset Field_Layout083, .LField_Layout083_GRID
	layout_offset Field_Layout083, .LField_Layout083_ANIMATION
	layout_offset Field_Layout083, .LField_Layout083_BLEND
	layout_offset Field_Layout083, .LField_Layout083_MARKERS
.LField_Layout083_METATILES:
	.incbin "games/tbs/src/layout/layout_083/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout083_SHAPES:
	.incbin "games/tbs/src/layout/layout_083/shapes.table.lz"
	.balign 4, 0
.LField_Layout083_GRID:
	.incbin "games/tbs/src/layout/layout_083/grid.parts.lz"
	.balign 4, 0
.LField_Layout083_ANIMATION:
	.incbin "games/tbs/src/layout/layout_080/animation.script.lz"
	.balign 4, 0
.LField_Layout083_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout083_MARKERS:
	.incbin "games/tbs/src/layout/layout_083/markers.table"
	.balign 4, 0
