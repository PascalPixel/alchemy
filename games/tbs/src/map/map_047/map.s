	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map047
Field_Map047:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 8, 8, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map047, .LField_Map047_METATILES
	map_block Field_Map047, .LField_Map047_SHAPES
	map_block Field_Map047, .LField_Map047_GRID
	map_block Field_Map047, .LField_Map047_ANIMATION
	map_block Field_Map047, .LField_Map047_BLEND
	map_block Field_Map047, .LField_Map047_MARKERS
.LField_Map047_METATILES:
	.incbin "map/map_047/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map047_SHAPES:
	.incbin "map/map_047/shapes.table.lz"
	.balign 4, 0
.LField_Map047_GRID:
	.incbin "map/map_047/grid.parts.lz"
	.balign 4, 0
.LField_Map047_ANIMATION:
	.incbin "map/map_037/animation.script.lz"
	.balign 4, 0
.LField_Map047_BLEND:
	.incbin "map/map_047/blend.script.lz"
	.balign 4, 0
.LField_Map047_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
