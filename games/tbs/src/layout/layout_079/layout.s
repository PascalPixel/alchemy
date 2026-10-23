	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout079
Field_Layout079:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout079, .LField_Layout079_METATILES
	layout_offset Field_Layout079, .LField_Layout079_SHAPES
	layout_offset Field_Layout079, .LField_Layout079_GRID
	layout_offset Field_Layout079, .LField_Layout079_ANIMATION
	layout_offset Field_Layout079, .LField_Layout079_BLEND
	layout_offset Field_Layout079, .LField_Layout079_MARKERS
.LField_Layout079_METATILES:
	.incbin "games/tbs/src/layout/layout_079/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout079_SHAPES:
	.incbin "games/tbs/src/layout/layout_079/shapes.table.lz"
	.balign 4, 0
.LField_Layout079_GRID:
	.incbin "games/tbs/src/layout/layout_079/grid.parts.lz"
	.balign 4, 0
.LField_Layout079_ANIMATION:
	.incbin "games/tbs/src/layout/layout_078/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout079_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout079_MARKERS:
	.incbin "games/tbs/src/layout/layout_079/treasure_spots.table"
	.balign 4, 0
