	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout100
Field_Layout100:
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout100, .LField_Layout100_METATILES
	layout_offset Field_Layout100, .LField_Layout100_SHAPES
	layout_offset Field_Layout100, .LField_Layout100_GRID
	layout_offset Field_Layout100, .LField_Layout100_ANIMATION
	layout_offset Field_Layout100, .LField_Layout100_BLEND
	layout_offset Field_Layout100, .LField_Layout100_MARKERS
.LField_Layout100_METATILES:
	.incbin "games/tbs/src/layout/layout_100/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout100_SHAPES:
	.incbin "games/tbs/src/layout/layout_100/shapes.table.lz"
	.balign 4, 0
.LField_Layout100_GRID:
	.incbin "games/tbs/src/layout/layout_100/map.grid.lz"
	.balign 4, 0
.LField_Layout100_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout100_BLEND:
	.incbin "games/tbs/src/layout/layout_063/blend.script.lz"
	.balign 4, 0
.LField_Layout100_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette10
Tileset_Palette10:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_10.gbapal.lz"
