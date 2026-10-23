	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map100
Field_Map100:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map100, .LField_Map100_METATILES
	map_block Field_Map100, .LField_Map100_SHAPES
	map_block Field_Map100, .LField_Map100_GRID
	map_block Field_Map100, .LField_Map100_ANIMATION
	map_block Field_Map100, .LField_Map100_BLEND
	map_block Field_Map100, .LField_Map100_MARKERS
.LField_Map100_METATILES:
	.incbin "games/tbs/src/layout/layout_100/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map100_SHAPES:
	.incbin "games/tbs/src/layout/layout_100/shapes.table.lz"
	.balign 4, 0
.LField_Map100_GRID:
	.incbin "games/tbs/src/layout/layout_100/grid.parts.lz"
	.balign 4, 0
.LField_Map100_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/animation.script.lz"
	.balign 4, 0
.LField_Map100_BLEND:
	.incbin "games/tbs/src/layout/layout_063/blend.script.lz"
	.balign 4, 0
.LField_Map100_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette10
Tileset_Palette10:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_10.gbapal.lz"
