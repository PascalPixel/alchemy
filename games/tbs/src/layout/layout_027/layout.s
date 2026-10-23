	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map027
Field_Map027:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map027, .LField_Map027_METATILES
	map_block Field_Map027, .LField_Map027_SHAPES
	map_block Field_Map027, .LField_Map027_GRID
	map_block Field_Map027, .LField_Map027_ANIMATION
	map_block Field_Map027, .LField_Map027_BLEND
	map_block Field_Map027, .LField_Map027_MARKERS
.LField_Map027_METATILES:
	.incbin "games/tbs/src/layout/layout_027/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map027_SHAPES:
	.incbin "games/tbs/src/layout/layout_027/shapes.table.lz"
	.balign 4, 0
.LField_Map027_GRID:
	.incbin "games/tbs/src/layout/layout_027/grid.parts.lz"
	.balign 4, 0
.LField_Map027_ANIMATION:
	.incbin "games/tbs/src/layout/layout_025/animation.script.lz"
	.balign 4, 0
.LField_Map027_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Map027_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
