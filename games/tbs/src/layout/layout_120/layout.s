	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout120
Field_Layout120:
	layout_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout120, .LField_Layout120_METATILES
	layout_offset Field_Layout120, .LField_Layout120_SHAPES
	layout_offset Field_Layout120, .LField_Layout120_GRID
	layout_offset Field_Layout120, .LField_Layout120_ANIMATION
	layout_offset Field_Layout120, .LField_Layout120_BLEND
	layout_offset Field_Layout120, .LField_Layout120_MARKERS
.LField_Layout120_METATILES:
	.incbin "games/tbs/src/layout/layout_120/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout120_SHAPES:
	.incbin "games/tbs/src/layout/layout_120/shapes.table.lz"
	.balign 4, 0
.LField_Layout120_GRID:
	.incbin "games/tbs/src/layout/layout_120/map.grid.lz"
	.balign 4, 0
.LField_Layout120_ANIMATION:
	.incbin "games/tbs/src/layout/layout_094/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout120_BLEND:
	.incbin "games/tbs/src/layout/layout_018/blend.script.lz"
	.balign 4, 0
.LField_Layout120_MARKERS:
	.incbin "games/tbs/src/layout/layout_120/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette26
Tileset_Palette26:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_26.gbapal.lz"
	.byte 0x9c, 0x00	@ PACKER: left in the buffer by Field_Layout120+0x1de
