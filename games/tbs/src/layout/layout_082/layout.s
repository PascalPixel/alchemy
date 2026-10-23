	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map082
Field_Map082:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map082, .LField_Map082_METATILES
	map_block Field_Map082, .LField_Map082_SHAPES
	map_block Field_Map082, .LField_Map082_GRID
	map_block Field_Map082, .LField_Map082_ANIMATION
	map_block Field_Map082, .LField_Map082_BLEND
	map_block Field_Map082, .LField_Map082_MARKERS
.LField_Map082_METATILES:
	.incbin "games/tbs/src/layout/layout_082/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map082_SHAPES:
	.incbin "games/tbs/src/layout/layout_082/shapes.table.lz"
	.balign 4, 0
.LField_Map082_GRID:
	.incbin "games/tbs/src/layout/layout_082/grid.parts.lz"
	.balign 4, 0
.LField_Map082_ANIMATION:
	.incbin "games/tbs/src/layout/layout_078/animation.script.lz"
	.balign 4, 0
.LField_Map082_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Map082_MARKERS:
	.incbin "games/tbs/src/layout/layout_082/markers.table"
	.balign 4, 0
