	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map043
Field_Map043:
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map043, .LField_Map043_METATILES
	map_block Field_Map043, .LField_Map043_SHAPES
	map_block Field_Map043, .LField_Map043_GRID
	map_block Field_Map043, .LField_Map043_ANIMATION
	map_block Field_Map043, .LField_Map043_BLEND
	map_block Field_Map043, .LField_Map043_MARKERS
.LField_Map043_METATILES:
	.incbin "map/map_043/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map043_SHAPES:
	.incbin "map/map_043/shapes.table.lz"
	.balign 4, 0
.LField_Map043_GRID:
	.incbin "map/map_043/grid.parts.lz"
	.balign 4, 0
.LField_Map043_ANIMATION:
	.incbin "map/map_043/animation.script.lz"
	.balign 4, 0
.LField_Map043_BLEND:
	.incbin "map/map_039/blend.script.lz"
	.balign 4, 0
.LField_Map043_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
