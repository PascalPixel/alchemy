	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout138
Field_Layout138:
	layout_header 0, 0, 56, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout138, .LField_Layout138_METATILES
	layout_offset Field_Layout138, .LField_Layout138_SHAPES
	layout_offset Field_Layout138, .LField_Layout138_GRID
	layout_offset Field_Layout138, .LField_Layout138_ANIMATION
	layout_offset Field_Layout138, .LField_Layout138_BLEND
	layout_offset Field_Layout138, .LField_Layout138_MARKERS
.LField_Layout138_METATILES:
	.incbin "games/tbs/src/layout/layout_138/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout138_SHAPES:
	.incbin "games/tbs/src/layout/layout_138/shapes.table.lz"
	.balign 4, 0
.LField_Layout138_GRID:
	.incbin "games/tbs/src/layout/layout_138/map.grid.lz"
	.balign 4, 0
.LField_Layout138_ANIMATION:
	.incbin "games/tbs/src/layout/layout_031/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout138_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout138_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette35
Tileset_Palette35:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_35.gbapal.lz"
