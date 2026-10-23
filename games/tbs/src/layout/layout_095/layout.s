	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout095
Field_Layout095:
	layout_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout095, .LField_Layout095_METATILES
	layout_offset Field_Layout095, .LField_Layout095_SHAPES
	layout_offset Field_Layout095, .LField_Layout095_GRID
	layout_offset Field_Layout095, .LField_Layout095_ANIMATION
	layout_offset Field_Layout095, .LField_Layout095_BLEND
	layout_offset Field_Layout095, .LField_Layout095_MARKERS
.LField_Layout095_METATILES:
	.incbin "games/tbs/src/layout/layout_095/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout095_SHAPES:
	.incbin "games/tbs/src/layout/layout_095/shapes.table.lz"
	.balign 4, 0
.LField_Layout095_GRID:
	.incbin "games/tbs/src/layout/layout_095/grid.parts.lz"
	.balign 4, 0
.LField_Layout095_ANIMATION:
	.incbin "games/tbs/src/layout/layout_094/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout095_BLEND:
	.incbin "games/tbs/src/layout/layout_018/blend.script.lz"
	.balign 4, 0
.LField_Layout095_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
