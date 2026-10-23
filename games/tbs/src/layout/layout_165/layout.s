	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout165
Field_Layout165:
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout165, .LField_Layout165_METATILES
	layout_offset Field_Layout165, .LField_Layout165_SHAPES
	layout_offset Field_Layout165, .LField_Layout165_GRID
	layout_offset Field_Layout165, .LField_Layout165_ANIMATION
	layout_offset Field_Layout165, .LField_Layout165_BLEND
	layout_offset Field_Layout165, .LField_Layout165_MARKERS
.LField_Layout165_METATILES:
	.incbin "games/tbs/src/layout/layout_165/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout165_SHAPES:
	.incbin "games/tbs/src/layout/layout_165/shapes.table.lz"
	.balign 4, 0
.LField_Layout165_GRID:
	.incbin "games/tbs/src/layout/layout_165/grid.parts.lz"
	.balign 4, 0
.LField_Layout165_ANIMATION:
	.incbin "games/tbs/src/layout/layout_164/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout165_BLEND:
	.incbin "games/tbs/src/layout/layout_164/blend.script.lz"
	.balign 4, 0
.LField_Layout165_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
