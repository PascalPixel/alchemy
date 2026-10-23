	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout171
Field_Layout171:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout171, .LField_Layout171_METATILES
	layout_offset Field_Layout171, .LField_Layout171_SHAPES
	layout_offset Field_Layout171, .LField_Layout171_GRID
	layout_offset Field_Layout171, .LField_Layout171_ANIMATION
	layout_offset Field_Layout171, .LField_Layout171_BLEND
	layout_offset Field_Layout171, .LField_Layout171_MARKERS
.LField_Layout171_METATILES:
	.incbin "games/tbs/src/layout/layout_171/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout171_SHAPES:
	.incbin "games/tbs/src/layout/layout_171/shapes.table.lz"
	.balign 4, 0
.LField_Layout171_GRID:
	.incbin "games/tbs/src/layout/layout_171/grid.parts.lz"
	.balign 4, 0
.LField_Layout171_ANIMATION:
	.incbin "games/tbs/src/layout/layout_171/animation.script.lz"
	.balign 4, 0
.LField_Layout171_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout171_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette53
Tileset_Palette53:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_53.gbapal.lz"
	.byte 0x1c	@ PACKER: left in the buffer by Field_Layout171+0x1c7
