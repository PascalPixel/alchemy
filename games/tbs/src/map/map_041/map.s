	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map041
Field_Map041:
	map_header 0, 0, 64, 84, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map041, .LField_Map041_METATILES
	map_block Field_Map041, .LField_Map041_SHAPES
	map_block Field_Map041, .LField_Map041_GRID
	map_block Field_Map041, .LField_Map041_ANIMATION
	map_block Field_Map041
	map_block Field_Map041, .LField_Map041_MARKERS
.LField_Map041_METATILES:
	.incbin "map/map_041/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map041_SHAPES:
	.incbin "map/map_041/shapes.table.lz"
	.balign 4, 0
.LField_Map041_GRID:
	.incbin "map/map_041/grid.parts.lz"
	.balign 4, 0
.LField_Map041_ANIMATION:
	.incbin "map/map_041/animation.script.lz"
	.balign 4, 0
.LField_Map041_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
