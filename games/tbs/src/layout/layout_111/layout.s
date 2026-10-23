	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout111
Field_Layout111:
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout111, .LField_Layout111_METATILES
	layout_offset Field_Layout111, .LField_Layout111_SHAPES
	layout_offset Field_Layout111, .LField_Layout111_GRID
	layout_offset Field_Layout111, .LField_Layout111_ANIMATION
	layout_offset Field_Layout111, .LField_Layout111_BLEND
	layout_offset Field_Layout111, .LField_Layout111_MARKERS
.LField_Layout111_METATILES:
	.incbin "games/tbs/src/layout/layout_111/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout111_SHAPES:
	.incbin "games/tbs/src/layout/layout_111/shapes.table.lz"
	.balign 4, 0
.LField_Layout111_GRID:
	.incbin "games/tbs/src/layout/layout_111/grid.parts.lz"
	.balign 4, 0
.LField_Layout111_ANIMATION:
	.incbin "games/tbs/src/layout/layout_111/animation.script.lz"
	.balign 4, 0
.LField_Layout111_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout111_MARKERS:
	.incbin "games/tbs/src/layout/layout_111/markers.table"
	.balign 4, 0
	.global Tileset_Palette16
Tileset_Palette16:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_16.gbapal.lz"
	.byte 0xc7, 0x0c, 0xb4	@ PACKER: left in the buffer by Field_Layout111+0x179
