	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout121
Field_Layout121:
	layout_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout121, .LField_Layout121_METATILES
	layout_offset Field_Layout121, .LField_Layout121_SHAPES
	layout_offset Field_Layout121, .LField_Layout121_GRID
	layout_offset Field_Layout121, .LField_Layout121_ANIMATION
	layout_offset Field_Layout121, .LField_Layout121_BLEND
	layout_offset Field_Layout121, .LField_Layout121_MARKERS
.LField_Layout121_METATILES:
	.incbin "games/tbs/src/layout/layout_121/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout121_SHAPES:
	.incbin "games/tbs/src/layout/layout_121/shapes.table.lz"
	.balign 4, 0
.LField_Layout121_GRID:
	.incbin "games/tbs/src/layout/layout_121/grid.parts.lz"
	.balign 4, 0
.LField_Layout121_ANIMATION:
	.incbin "games/tbs/src/layout/layout_094/animation.script.lz"
	.balign 4, 0
.LField_Layout121_BLEND:
	.incbin "games/tbs/src/layout/layout_018/blend.script.lz"
	.balign 4, 0
.LField_Layout121_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
