	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout155
Field_Layout155:
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout155, .LField_Layout155_METATILES
	layout_offset Field_Layout155, .LField_Layout155_SHAPES
	layout_offset Field_Layout155, .LField_Layout155_GRID
	layout_offset Field_Layout155, .LField_Layout155_ANIMATION
	layout_offset Field_Layout155, .LField_Layout155_BLEND
	layout_offset Field_Layout155, .LField_Layout155_MARKERS
.LField_Layout155_METATILES:
	.incbin "games/tbs/src/layout/layout_155/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout155_SHAPES:
	.incbin "games/tbs/src/layout/layout_155/shapes.table.lz"
	.balign 4, 0
.LField_Layout155_GRID:
	.incbin "games/tbs/src/layout/layout_155/grid.parts.lz"
	.balign 4, 0
.LField_Layout155_ANIMATION:
	.incbin "games/tbs/src/layout/layout_054/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout155_BLEND:
	.incbin "games/tbs/src/layout/layout_054/blend.script.lz"
	.balign 4, 0
.LField_Layout155_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette43
Tileset_Palette43:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_43.gbapal.lz"
	.byte 0xec, 0x5c	@ PACKER: left in the buffer by Field_Layout155+0x17e
