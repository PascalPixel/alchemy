	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout086
Field_Layout086:
	layout_header 0, 0, 100, 68, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout086, .LField_Layout086_METATILES
	layout_offset Field_Layout086, .LField_Layout086_SHAPES
	layout_offset Field_Layout086, .LField_Layout086_GRID
	layout_offset Field_Layout086, .LField_Layout086_ANIMATION
	layout_offset Field_Layout086, .LField_Layout086_BLEND
	layout_offset Field_Layout086, .LField_Layout086_MARKERS
.LField_Layout086_METATILES:
	.incbin "games/tbs/src/layout/layout_086/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout086_SHAPES:
	.incbin "games/tbs/src/layout/layout_086/shapes.table.lz"
	.balign 4, 0
.LField_Layout086_GRID:
	.incbin "games/tbs/src/layout/layout_086/grid.parts.lz"
	.balign 4, 0
.LField_Layout086_ANIMATION:
	.incbin "games/tbs/src/layout/layout_086/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout086_BLEND:
	.incbin "games/tbs/src/layout/layout_086/blend.script.lz"
	.balign 4, 0
.LField_Layout086_MARKERS:
	.incbin "games/tbs/src/layout/layout_086/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette06
Tileset_Palette06:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_06.gbapal.lz"
	.byte 0x2a, 0x02, 0x98	@ PACKER: left in the buffer by Field_Layout086+0x15d
