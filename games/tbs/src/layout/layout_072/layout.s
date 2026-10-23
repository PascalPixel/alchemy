	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map072
Field_Map072:
	map_header 10, 4, 67, 67, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map072, .LField_Map072_METATILES
	map_block Field_Map072, .LField_Map072_SHAPES
	map_block Field_Map072, .LField_Map072_GRID
	map_block Field_Map072, .LField_Map072_ANIMATION
	map_block Field_Map072, .LField_Map072_BLEND
	map_block Field_Map072, .LField_Map072_MARKERS
.LField_Map072_METATILES:
	.incbin "games/tbs/src/layout/layout_072/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map072_SHAPES:
	.incbin "games/tbs/src/layout/layout_072/shapes.table.lz"
	.balign 4, 0
.LField_Map072_GRID:
	.incbin "games/tbs/src/layout/layout_072/grid.parts.lz"
	.balign 4, 0
.LField_Map072_ANIMATION:
	.incbin "games/tbs/src/layout/layout_028/animation.script.lz"
	.balign 4, 0
.LField_Map072_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Map072_MARKERS:
	.incbin "games/tbs/src/layout/layout_072/markers.table"
	.balign 4, 0
