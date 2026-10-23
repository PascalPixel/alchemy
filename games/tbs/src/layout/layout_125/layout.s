	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout125
Field_Layout125:
	layout_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout125, .LField_Layout125_METATILES
	layout_offset Field_Layout125, .LField_Layout125_SHAPES
	layout_offset Field_Layout125, .LField_Layout125_GRID
	layout_offset Field_Layout125, .LField_Layout125_ANIMATION
	layout_offset Field_Layout125, .LField_Layout125_BLEND
	layout_offset Field_Layout125, .LField_Layout125_MARKERS
.LField_Layout125_METATILES:
	.incbin "games/tbs/src/layout/layout_125/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout125_SHAPES:
	.incbin "games/tbs/src/layout/layout_125/shapes.table.lz"
	.balign 4, 0
.LField_Layout125_GRID:
	.incbin "games/tbs/src/layout/layout_125/map.grid.lz"
	.balign 4, 0
.LField_Layout125_ANIMATION:
	.incbin "games/tbs/src/layout/layout_125/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout125_BLEND:
	.incbin "games/tbs/src/layout/layout_125/blend.script.lz"
	.balign 4, 0
.LField_Layout125_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette29
Tileset_Palette29:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_29.gbapal.lz"
	.byte 0x9d, 0xe1, 0xfd	@ PACKER: left in the buffer by Field_Layout125+0x189
