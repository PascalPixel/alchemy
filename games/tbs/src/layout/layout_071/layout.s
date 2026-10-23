	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout071
Field_Layout071:
	layout_header 0, 0, 108, 90, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout071, .LField_Layout071_METATILES
	layout_offset Field_Layout071, .LField_Layout071_SHAPES
	layout_offset Field_Layout071, .LField_Layout071_GRID
	layout_offset Field_Layout071
	layout_offset Field_Layout071
	layout_offset Field_Layout071, .LField_Layout071_MARKERS
.LField_Layout071_METATILES:
	.incbin "games/tbs/src/layout/layout_071/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout071_SHAPES:
	.incbin "games/tbs/src/layout/layout_042/shapes.table.lz"
	.balign 4, 0
.LField_Layout071_GRID:
	.incbin "games/tbs/src/layout/layout_071/map.grid.lz"
	.balign 4, 0
.LField_Layout071_MARKERS:
	.incbin "games/tbs/src/layout/layout_071/treasure_spots.table"
	.balign 4, 0
