	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map150
Field_Map150:
	map_header 0, 0, 48, 48, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map150, .LField_Map150_METATILES
	map_block Field_Map150, .LField_Map150_SHAPES
	map_block Field_Map150, .LField_Map150_GRID
	map_block Field_Map150, .LField_Map150_ANIMATION
	map_block Field_Map150
	map_block Field_Map150, .LField_Map150_MARKERS
.LField_Map150_METATILES:
	.incbin "map/map_150/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map150_SHAPES:
	.incbin "map/map_150/shapes.table.lz"
	.balign 4, 0
.LField_Map150_GRID:
	.incbin "map/map_150/grid.parts.lz"
	.balign 4, 0
.LField_Map150_ANIMATION:
	.incbin "map/map_150/animation.script.lz"
	.balign 4, 0
.LField_Map150_MARKERS:
	.incbin "map/map_150/markers.table"
