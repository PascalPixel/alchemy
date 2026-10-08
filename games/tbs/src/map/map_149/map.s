	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map149
Field_Map149:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map149, .LField_Map149_METATILES
	map_block Field_Map149, .LField_Map149_SHAPES
	map_block Field_Map149, .LField_Map149_GRID
	map_block Field_Map149, .LField_Map149_ANIMATION
	map_block Field_Map149, .LField_Map149_BLEND
	map_block Field_Map149, .LField_Map149_MARKERS
.LField_Map149_METATILES:
	.incbin "map/map_149/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map149_SHAPES:
	.incbin "common/map/map_130/shapes.table.lz"
	.balign 4, 0
.LField_Map149_GRID:
	.incbin "map/map_149/grid.parts.lz"
	.balign 4, 0
.LField_Map149_ANIMATION:
	.incbin "map/map_080/animation.script.lz"
	.balign 4, 0
.LField_Map149_BLEND:
	.incbin "map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map149_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
