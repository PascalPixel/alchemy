	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map013
Field_Map013:
	map_header 0, 0, 98, 88, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 88, 16, 16, 0, 0, 0xff, 0xff
	map_layer 98, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map013, .LField_Map013_METATILES
	map_block Field_Map013, .LField_Map013_SHAPES
	map_block Field_Map013, .LField_Map013_GRID
	map_block Field_Map013, .LField_Map013_ANIMATION
	map_block Field_Map013, .LField_Map013_BLEND
	map_block Field_Map013, .LField_Map013_MARKERS
.LField_Map013_METATILES:
	.incbin "map/map_013/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map013_SHAPES:
	.incbin "common/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map013_GRID:
	.incbin "map/map_013/grid.parts.lz"
	.balign 4, 0
.LField_Map013_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map013_BLEND:
	.incbin "map/map_012/blend.script.lz"
	.balign 4, 0
.LField_Map013_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
