	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout123
Field_Layout123:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout123, .LField_Layout123_METATILES
	layout_offset Field_Layout123, .LField_Layout123_SHAPES
	layout_offset Field_Layout123, .LField_Layout123_GRID
	layout_offset Field_Layout123, .LField_Layout123_ANIMATION
	layout_offset Field_Layout123, .LField_Layout123_BLEND
	layout_offset Field_Layout123, .LField_Layout123_MARKERS
.LField_Layout123_METATILES:
	.incbin "games/tbs/src/layout/layout_123/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout123_SHAPES:
	.incbin "games/tbs/src/layout/layout_123/shapes.table.lz"
	.balign 4, 0
.LField_Layout123_GRID:
	.incbin "games/tbs/src/layout/layout_123/grid.parts.lz"
	.balign 4, 0
.LField_Layout123_ANIMATION:
	.incbin "games/tbs/src/layout/layout_123/animation.script.lz"
	.balign 4, 0
.LField_Layout123_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout123_MARKERS:
	.incbin "games/tbs/src/layout/layout_123/markers.table"
	.global Tileset_Palette28
Tileset_Palette28:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_28.gbapal.lz"
	.byte 0x15, 0x30	@ PACKER: left in the buffer by Field_Layout123+0x186
