@ Field_Map069: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map069
Field_Map069:
	map_header 0, 0, 106, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map069, .LField_Map069_METATILES
	map_block Field_Map069, .LField_Map069_SHAPES
	map_block Field_Map069, .LField_Map069_GRID
	map_block Field_Map069, .LField_Map069_ANIMATION
	map_block Field_Map069
	map_block Field_Map069, .LField_Map069_MARKERS
.LField_Map069_METATILES:
	.incbin "MAP/MAP_069/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map069_SHAPES:
	.incbin "MAP/MAP_069/SHAPES.table.lz"
	.balign 4, 0
.LField_Map069_GRID:
	.incbin "MAP/MAP_069/GRID.parts.lz"
	.balign 4, 0
.LField_Map069_ANIMATION:
	.incbin "MAP/MAP_069/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map069_MARKERS:
	.incbin "MAP/MAP_069/MARKERS.table"
