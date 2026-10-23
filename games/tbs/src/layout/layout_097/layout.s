	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout097
Field_Layout097:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout097, .LField_Layout097_METATILES
	layout_offset Field_Layout097, .LField_Layout097_SHAPES
	layout_offset Field_Layout097, .LField_Layout097_GRID
	layout_offset Field_Layout097, .LField_Layout097_ANIMATION
	layout_offset Field_Layout097, .LField_Layout097_BLEND
	layout_offset Field_Layout097, .LField_Layout097_MARKERS
.LField_Layout097_METATILES:
	.incbin "games/tbs/src/layout/layout_097/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout097_SHAPES:
	.incbin "games/tbs/src/layout/layout_097/shapes.table.lz"
	.balign 4, 0
.LField_Layout097_GRID:
	.incbin "games/tbs/src/layout/layout_097/grid.parts.lz"
	.balign 4, 0
.LField_Layout097_ANIMATION:
	.incbin "games/tbs/src/layout/layout_097/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout097_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout097_MARKERS:
	.incbin "games/tbs/src/layout/layout_097/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette08
Tileset_Palette08:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_08.gbapal.lz"
