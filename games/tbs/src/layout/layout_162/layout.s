	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map162
Field_Map162:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map162, .LField_Map162_METATILES
	map_block Field_Map162, .LField_Map162_SHAPES
	map_block Field_Map162, .LField_Map162_GRID
	map_block Field_Map162, .LField_Map162_ANIMATION
	map_block Field_Map162, .LField_Map162_BLEND
	map_block Field_Map162, .LField_Map162_MARKERS
.LField_Map162_METATILES:
	.incbin "games/tbs/src/layout/layout_162/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map162_SHAPES:
	.incbin "games/tbs/src/layout/layout_162/shapes.table.lz"
	.balign 4, 0
.LField_Map162_GRID:
	.incbin "games/tbs/src/layout/layout_162/grid.parts.lz"
	.balign 4, 0
.LField_Map162_ANIMATION:
	.incbin "games/tbs/src/layout/layout_037/animation.script.lz"
	.balign 4, 0
.LField_Map162_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Map162_MARKERS:
	.incbin "games/tbs/src/layout/layout_162/markers.table"
	.balign 4, 0
