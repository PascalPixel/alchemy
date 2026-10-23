	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout114
Field_Layout114:
	layout_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout114, .LField_Layout114_METATILES
	layout_offset Field_Layout114, .LField_Layout114_SHAPES
	layout_offset Field_Layout114, .LField_Layout114_GRID
	layout_offset Field_Layout114, .LField_Layout114_ANIMATION
	layout_offset Field_Layout114, .LField_Layout114_BLEND
	layout_offset Field_Layout114, .LField_Layout114_MARKERS
.LField_Layout114_METATILES:
	.incbin "games/tbs/src/layout/layout_114/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout114_SHAPES:
	.incbin "games/tbs/src/layout/layout_114/shapes.table.lz"
	.balign 4, 0
.LField_Layout114_GRID:
	.incbin "games/tbs/src/layout/layout_114/grid.parts.lz"
	.balign 4, 0
.LField_Layout114_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/animation.script.lz"
	.balign 4, 0
.LField_Layout114_BLEND:
	.incbin "games/tbs/src/layout/layout_114/blend.script.lz"
	.balign 4, 0
.LField_Layout114_MARKERS:
	.incbin "games/tbs/src/layout/layout_114/markers.table"
	.balign 4, 0
	.global Tileset_Palette20
Tileset_Palette20:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_20.gbapal.lz"
