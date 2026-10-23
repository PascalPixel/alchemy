	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout089
Field_Layout089:
	layout_header 0, 0, 55, 68, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout089, .LField_Layout089_METATILES
	layout_offset Field_Layout089, .LField_Layout089_SHAPES
	layout_offset Field_Layout089, .LField_Layout089_GRID
	layout_offset Field_Layout089, .LField_Layout089_ANIMATION
	layout_offset Field_Layout089, .LField_Layout089_BLEND
	layout_offset Field_Layout089, .LField_Layout089_MARKERS
.LField_Layout089_METATILES:
	.incbin "games/tbs/src/layout/layout_089/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout089_SHAPES:
	.incbin "games/tbs/src/layout/layout_089/shapes.table.lz"
	.balign 4, 0
.LField_Layout089_GRID:
	.incbin "games/tbs/src/layout/layout_089/grid.parts.lz"
	.balign 4, 0
.LField_Layout089_ANIMATION:
	.incbin "games/tbs/src/layout/layout_028/animation.script.lz"
	.balign 4, 0
.LField_Layout089_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout089_MARKERS:
	.incbin "games/tbs/src/layout/layout_089/markers.table"
	.balign 4, 0
