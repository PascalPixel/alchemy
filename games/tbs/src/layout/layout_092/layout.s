	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map092
Field_Map092:
	map_header 0, 0, 192, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map092, .LField_Map092_METATILES
	map_block Field_Map092, .LField_Map092_SHAPES
	map_block Field_Map092, .LField_Map092_GRID
	map_block Field_Map092, .LField_Map092_ANIMATION
	map_block Field_Map092
	map_block Field_Map092, .LField_Map092_MARKERS
.LField_Map092_METATILES:
	.incbin "games/tbs/src/layout/layout_092/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map092_SHAPES:
	.incbin "games/tbs/src/layout/layout_092/shapes.table.lz"
	.balign 4, 0
.LField_Map092_GRID:
	.incbin "games/tbs/src/layout/layout_092/grid.parts.lz"
	.balign 4, 0
.LField_Map092_ANIMATION:
	.incbin "games/tbs/src/layout/layout_091/animation.script.lz"
	.balign 4, 0
.LField_Map092_MARKERS:
	.incbin "games/tbs/src/layout/layout_092/markers.table"
	.balign 4, 0
