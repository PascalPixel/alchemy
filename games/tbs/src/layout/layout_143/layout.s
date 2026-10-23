	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout143
Field_Layout143:
	layout_header 0, 0, 72, 72, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout143, .LField_Layout143_METATILES
	layout_offset Field_Layout143, .LField_Layout143_SHAPES
	layout_offset Field_Layout143, .LField_Layout143_GRID
	layout_offset Field_Layout143, .LField_Layout143_ANIMATION
	layout_offset Field_Layout143, .LField_Layout143_BLEND
	layout_offset Field_Layout143, .LField_Layout143_MARKERS
.LField_Layout143_METATILES:
	.incbin "games/tbs/src/layout/layout_143/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout143_SHAPES:
	.incbin "games/tbs/src/layout/layout_143/shapes.table.lz"
	.balign 4, 0
.LField_Layout143_GRID:
	.incbin "games/tbs/src/layout/layout_143/grid.parts.lz"
	.balign 4, 0
.LField_Layout143_ANIMATION:
	.incbin "games/tbs/src/layout/layout_028/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout143_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout143_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette37
Tileset_Palette37:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_37.gbapal.lz"
