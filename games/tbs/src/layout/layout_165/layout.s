	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map165
Field_Map165:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map165, .LField_Map165_METATILES
	map_block Field_Map165, .LField_Map165_SHAPES
	map_block Field_Map165, .LField_Map165_GRID
	map_block Field_Map165, .LField_Map165_ANIMATION
	map_block Field_Map165, .LField_Map165_BLEND
	map_block Field_Map165, .LField_Map165_MARKERS
.LField_Map165_METATILES:
	.incbin "games/tbs/src/layout/layout_165/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map165_SHAPES:
	.incbin "games/tbs/src/layout/layout_165/shapes.table.lz"
	.balign 4, 0
.LField_Map165_GRID:
	.incbin "games/tbs/src/layout/layout_165/grid.parts.lz"
	.balign 4, 0
.LField_Map165_ANIMATION:
	.incbin "games/tbs/src/layout/layout_164/animation.script.lz"
	.balign 4, 0
.LField_Map165_BLEND:
	.incbin "games/tbs/src/layout/layout_164/blend.script.lz"
	.balign 4, 0
.LField_Map165_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
