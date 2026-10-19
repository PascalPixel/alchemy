	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map057
Field_Map057:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map057, .LField_Map057_METATILES
	map_block Field_Map057, .LField_Map057_SHAPES
	map_block Field_Map057, .LField_Map057_GRID
	map_block Field_Map057, .LField_Map057_ANIMATION
	map_block Field_Map057, .LField_Map057_BLEND
	map_block Field_Map057, .LField_Map057_MARKERS
.LField_Map057_METATILES:
	.incbin "games/tbs/src/map/map_057/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map057_SHAPES:
	.incbin "games/tbs/src/map/map_057/shapes.table.lz"
	.balign 4, 0
.LField_Map057_GRID:
	.incbin "games/tbs/src/map/map_057/grid.parts.lz"
	.balign 4, 0
.LField_Map057_ANIMATION:
	.incbin "games/tbs/src/map/map_054/animation.script.lz"
	.balign 4, 0
.LField_Map057_BLEND:
	.incbin "games/tbs/src/map/map_054/blend.script.lz"
	.balign 4, 0
.LField_Map057_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
