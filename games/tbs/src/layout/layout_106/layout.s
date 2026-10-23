	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout106
Field_Layout106:
	layout_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout106, .LField_Layout106_METATILES
	layout_offset Field_Layout106, .LField_Layout106_SHAPES
	layout_offset Field_Layout106, .LField_Layout106_GRID
	layout_offset Field_Layout106, .LField_Layout106_ANIMATION
	layout_offset Field_Layout106
	layout_offset Field_Layout106, .LField_Layout106_MARKERS
.LField_Layout106_METATILES:
	.incbin "games/tbs/src/layout/layout_106/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout106_SHAPES:
	.incbin "games/tbs/src/layout/layout_106/shapes.table.lz"
	.balign 4, 0
.LField_Layout106_GRID:
	.incbin "games/tbs/src/layout/layout_106/grid.parts.lz"
	.balign 4, 0
.LField_Layout106_ANIMATION:
	.incbin "games/tbs/src/layout/layout_101/animation.script.lz"
	.balign 4, 0
.LField_Layout106_MARKERS:
	.incbin "games/tbs/src/layout/layout_106/markers.table"
	.global Tileset_Palette12
Tileset_Palette12:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_12.gbapal.lz"
	.byte 0x19, 0xe9, 0x78	@ PACKER: left in the buffer by Field_Layout106+0x1a5
