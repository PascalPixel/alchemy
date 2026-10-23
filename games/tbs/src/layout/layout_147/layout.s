	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout147
Field_Layout147:
	layout_header 0, 0, 64, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	layout_layer 128, 0, 20, 20, -121, 35, 0x1f, 0x1f
	layout_offset Field_Layout147, .LField_Layout147_METATILES
	layout_offset Field_Layout147, .LField_Layout147_SHAPES
	layout_offset Field_Layout147, .LField_Layout147_GRID
	layout_offset Field_Layout147, .LField_Layout147_ANIMATION
	layout_offset Field_Layout147
	layout_offset Field_Layout147, .LField_Layout147_MARKERS
.LField_Layout147_METATILES:
	.incbin "games/tbs/src/layout/layout_147/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout147_SHAPES:
	.incbin "games/tbs/src/layout/layout_147/shapes.table.lz"
	.balign 4, 0
.LField_Layout147_GRID:
	.incbin "games/tbs/src/layout/layout_147/map.grid.lz"
	.balign 4, 0
.LField_Layout147_ANIMATION:
	.incbin "games/tbs/src/layout/layout_147/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout147_MARKERS:
	.incbin "games/tbs/src/layout/layout_147/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette39
Tileset_Palette39:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_39.gbapal.lz"
