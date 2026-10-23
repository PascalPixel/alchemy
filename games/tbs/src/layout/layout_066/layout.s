	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map066
Field_Map066:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map066, .LField_Map066_METATILES
	map_block Field_Map066, .LField_Map066_SHAPES
	map_block Field_Map066, .LField_Map066_GRID
	map_block Field_Map066, .LField_Map066_ANIMATION
	map_block Field_Map066, .LField_Map066_BLEND
	map_block Field_Map066, .LField_Map066_MARKERS
.LField_Map066_METATILES:
	.incbin "games/tbs/src/layout/layout_066/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map066_SHAPES:
	.incbin "games/tbs/src/layout/layout_066/shapes.table.lz"
	.balign 4, 0
.LField_Map066_GRID:
	.incbin "games/tbs/src/layout/layout_066/grid.parts.lz"
	.balign 4, 0
.LField_Map066_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/animation.script.lz"
	.balign 4, 0
.LField_Map066_BLEND:
	.incbin "games/tbs/src/layout/layout_063/blend.script.lz"
	.balign 4, 0
.LField_Map066_MARKERS:
	.incbin "games/tbs/src/layout/layout_066/markers.table"
	.balign 4, 0
