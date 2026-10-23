	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout043
Field_Layout043:
	layout_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout043, .LField_Layout043_METATILES
	layout_offset Field_Layout043, .LField_Layout043_SHAPES
	layout_offset Field_Layout043, .LField_Layout043_GRID
	layout_offset Field_Layout043, .LField_Layout043_ANIMATION
	layout_offset Field_Layout043, .LField_Layout043_BLEND
	layout_offset Field_Layout043, .LField_Layout043_MARKERS
.LField_Layout043_METATILES:
	.incbin "games/tbs/src/layout/layout_043/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout043_SHAPES:
	.incbin "games/tbs/src/layout/layout_043/shapes.table.lz"
	.balign 4, 0
.LField_Layout043_GRID:
	.incbin "games/tbs/src/layout/layout_043/grid.parts.lz"
	.balign 4, 0
.LField_Layout043_ANIMATION:
	.incbin "games/tbs/src/layout/layout_043/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout043_BLEND:
	.incbin "games/tbs/src/layout/layout_039/blend.script.lz"
	.balign 4, 0
.LField_Layout043_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette02
Tileset_Palette02:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_02.gbapal.lz"
	.byte 0xe3, 0x65	@ PACKER: left in the buffer by Field_Layout043+0x166
