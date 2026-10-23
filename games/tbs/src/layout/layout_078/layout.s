	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map078
Field_Map078:
	map_header 0, 0, 64, 64, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map078, .LField_Map078_METATILES
	map_block Field_Map078, .LField_Map078_SHAPES
	map_block Field_Map078, .LField_Map078_GRID
	map_block Field_Map078, .LField_Map078_ANIMATION
	map_block Field_Map078, .LField_Map078_BLEND
	map_block Field_Map078, .LField_Map078_MARKERS
.LField_Map078_METATILES:
	.incbin "games/tbs/src/layout/layout_078/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map078_SHAPES:
	.incbin "games/tbs/src/layout/layout_078/shapes.table.lz"
	.balign 4, 0
.LField_Map078_GRID:
	.incbin "games/tbs/src/layout/layout_078/grid.parts.lz"
	.balign 4, 0
.LField_Map078_ANIMATION:
	.incbin "games/tbs/src/layout/layout_078/animation.script.lz"
	.balign 4, 0
.LField_Map078_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Map078_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
