	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map026
Field_Map026:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map026, .LField_Map026_METATILES
	map_block Field_Map026, .LField_Map026_SHAPES
	map_block Field_Map026, .LField_Map026_GRID
	map_block Field_Map026, .LField_Map026_ANIMATION
	map_block Field_Map026, .LField_Map026_BLEND
	map_block Field_Map026, .LField_Map026_MARKERS
.LField_Map026_METATILES:
	.incbin "games/tbs/src/map/map_026/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map026_SHAPES:
	.incbin "games/tbs/src/map/map_026/shapes.table.lz"
	.balign 4, 0
.LField_Map026_GRID:
	.incbin "games/tbs/src/map/map_026/grid.parts.lz"
	.balign 4, 0
.LField_Map026_ANIMATION:
	.incbin "games/tbs/src/map/map_025/animation.script.lz"
	.balign 4, 0
.LField_Map026_BLEND:
	.incbin "games/tbs/src/map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map026_MARKERS:
	.incbin "games/tbs/src/map/map_026/markers.table"
	.balign 4, 0
