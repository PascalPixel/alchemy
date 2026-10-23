	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map095
Field_Map095:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map095, .LField_Map095_METATILES
	map_block Field_Map095, .LField_Map095_SHAPES
	map_block Field_Map095, .LField_Map095_GRID
	map_block Field_Map095, .LField_Map095_ANIMATION
	map_block Field_Map095, .LField_Map095_BLEND
	map_block Field_Map095, .LField_Map095_MARKERS
.LField_Map095_METATILES:
	.incbin "games/tbs/src/layout/layout_095/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map095_SHAPES:
	.incbin "games/tbs/src/layout/layout_095/shapes.table.lz"
	.balign 4, 0
.LField_Map095_GRID:
	.incbin "games/tbs/src/layout/layout_095/grid.parts.lz"
	.balign 4, 0
.LField_Map095_ANIMATION:
	.incbin "games/tbs/src/layout/layout_094/animation.script.lz"
	.balign 4, 0
.LField_Map095_BLEND:
	.incbin "games/tbs/src/layout/layout_018/blend.script.lz"
	.balign 4, 0
.LField_Map095_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
