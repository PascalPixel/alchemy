	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout154
Field_Layout154:
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout154, .LField_Layout154_METATILES
	layout_offset Field_Layout154, .LField_Layout154_SHAPES
	layout_offset Field_Layout154, .LField_Layout154_GRID
	layout_offset Field_Layout154, .LField_Layout154_ANIMATION
	layout_offset Field_Layout154, .LField_Layout154_BLEND
	layout_offset Field_Layout154, .LField_Layout154_MARKERS
.LField_Layout154_METATILES:
	.incbin "games/tbs/src/layout/layout_154/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout154_SHAPES:
	.incbin "games/tbs/src/layout/layout_154/shapes.table.lz"
	.balign 4, 0
.LField_Layout154_GRID:
	.incbin "games/tbs/src/layout/layout_154/map.grid.lz"
	.balign 4, 0
.LField_Layout154_ANIMATION:
	.incbin "games/tbs/src/layout/layout_154/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout154_BLEND:
	.incbin "games/tbs/src/layout/layout_154/blend.script.lz"
	.balign 4, 0
.LField_Layout154_MARKERS:
	.incbin "games/tbs/src/layout/layout_154/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette42
Tileset_Palette42:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_42.gbapal.lz"
	.byte 0x94, 0x1c	@ PACKER: left in the buffer by Field_Layout154+0x1b6
