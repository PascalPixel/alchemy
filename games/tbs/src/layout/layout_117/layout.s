	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout117
Field_Layout117:
	layout_header 0, 0, 45, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout117, .LField_Layout117_METATILES
	layout_offset Field_Layout117, .LField_Layout117_SHAPES
	layout_offset Field_Layout117, .LField_Layout117_GRID
	layout_offset Field_Layout117, .LField_Layout117_ANIMATION
	layout_offset Field_Layout117
	layout_offset Field_Layout117, .LField_Layout117_MARKERS
.LField_Layout117_METATILES:
	.incbin "games/tbs/src/layout/layout_117/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout117_SHAPES:
	.incbin "games/tbs/src/layout/layout_117/shapes.table.lz"
	.balign 4, 0
.LField_Layout117_GRID:
	.incbin "games/tbs/src/layout/layout_117/grid.parts.lz"
	.balign 4, 0
.LField_Layout117_ANIMATION:
	.incbin "games/tbs/src/layout/layout_103/animation.script.lz"
	.balign 4, 0
.LField_Layout117_MARKERS:
	.incbin "games/tbs/src/layout/layout_117/markers.table"
	.balign 4, 0
	.global Tileset_Palette23
Tileset_Palette23:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_23.gbapal.lz"
