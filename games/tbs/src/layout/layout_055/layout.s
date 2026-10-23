	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map055
Field_Map055:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map055, .LField_Map055_METATILES
	map_block Field_Map055, .LField_Map055_SHAPES
	map_block Field_Map055, .LField_Map055_GRID
	map_block Field_Map055, .LField_Map055_ANIMATION
	map_block Field_Map055, .LField_Map055_BLEND
	map_block Field_Map055, .LField_Map055_MARKERS
.LField_Map055_METATILES:
	.incbin "games/tbs/src/layout/layout_055/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map055_SHAPES:
	.incbin "games/tbs/src/layout/layout_055/shapes.table.lz"
	.balign 4, 0
.LField_Map055_GRID:
	.incbin "games/tbs/src/layout/layout_055/grid.parts.lz"
	.balign 4, 0
.LField_Map055_ANIMATION:
	.incbin "games/tbs/src/layout/layout_054/animation.script.lz"
	.balign 4, 0
.LField_Map055_BLEND:
	.incbin "games/tbs/src/layout/layout_054/blend.script.lz"
	.balign 4, 0
.LField_Map055_MARKERS:
	.incbin "games/tbs/src/layout/layout_055/markers.table"
	.balign 4, 0
