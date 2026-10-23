	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map058
Field_Map058:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map058, .LField_Map058_METATILES
	map_block Field_Map058, .LField_Map058_SHAPES
	map_block Field_Map058, .LField_Map058_GRID
	map_block Field_Map058, .LField_Map058_ANIMATION
	map_block Field_Map058, .LField_Map058_BLEND
	map_block Field_Map058, .LField_Map058_MARKERS
.LField_Map058_METATILES:
	.incbin "games/tbs/src/layout/layout_058/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map058_SHAPES:
	.incbin "games/tbs/src/layout/layout_058/shapes.table.lz"
	.balign 4, 0
.LField_Map058_GRID:
	.incbin "games/tbs/src/layout/layout_058/grid.parts.lz"
	.balign 4, 0
.LField_Map058_ANIMATION:
	.incbin "games/tbs/src/layout/layout_058/animation.script.lz"
	.balign 4, 0
.LField_Map058_BLEND:
	.incbin "games/tbs/src/layout/layout_058/blend.script.lz"
	.balign 4, 0
.LField_Map058_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
