	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout062
Field_Layout062:
	layout_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout062, .LField_Layout062_METATILES
	layout_offset Field_Layout062, .LField_Layout062_SHAPES
	layout_offset Field_Layout062, .LField_Layout062_GRID
	layout_offset Field_Layout062, .LField_Layout062_ANIMATION
	layout_offset Field_Layout062, .LField_Layout062_BLEND
	layout_offset Field_Layout062, .LField_Layout062_MARKERS
.LField_Layout062_METATILES:
	.incbin "games/tbs/src/layout/layout_062/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout062_SHAPES:
	.incbin "games/tbs/src/layout/layout_062/shapes.table.lz"
	.balign 4, 0
.LField_Layout062_GRID:
	.incbin "games/tbs/src/layout/layout_062/map.grid.lz"
	.balign 4, 0
.LField_Layout062_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout062_BLEND:
	.incbin "games/tbs/src/layout/layout_033/blend.script.lz"
	.balign 4, 0
.LField_Layout062_MARKERS:
	.incbin "games/tbs/src/layout/layout_062/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette03
Tileset_Palette03:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_03.gbapal.lz"
	.byte 0x7e, 0x5e	@ PACKER: left in the buffer by Field_Layout062+0x156
