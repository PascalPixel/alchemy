	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout168
Field_Layout168:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout168, .LField_Layout168_METATILES
	layout_offset Field_Layout168, .LField_Layout168_SHAPES
	layout_offset Field_Layout168, .LField_Layout168_GRID
	layout_offset Field_Layout168, .LField_Layout168_ANIMATION
	layout_offset Field_Layout168
	layout_offset Field_Layout168, .LField_Layout168_MARKERS
.LField_Layout168_METATILES:
	.incbin "games/tbs/src/layout/layout_168/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout168_SHAPES:
	.incbin "games/tbs/src/layout/layout_168/shapes.table.lz"
	.balign 4, 0
.LField_Layout168_GRID:
	.incbin "games/tbs/src/layout/layout_168/grid.parts.lz"
	.balign 4, 0
.LField_Layout168_ANIMATION:
	.incbin "games/tbs/src/layout/layout_054/animation.script.lz"
	.balign 4, 0
.LField_Layout168_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette52
Tileset_Palette52:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_52.gbapal.lz"
	.byte 0x06	@ PACKER: left in the buffer by Field_Layout168+0x103
