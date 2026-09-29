@ Field_Map004: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map004
Field_Map004:
	map_header 0, 0, 60, 70, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map004, .LField_Map004_METATILES
	map_block Field_Map004, .LField_Map004_SHAPES
	map_block Field_Map004, .LField_Map004_GRID
	map_block Field_Map004
	map_block Field_Map004
	map_block Field_Map004, .LField_Map004_MARKERS
.LField_Map004_METATILES:
	.incbin "MAP/MAP_004/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map004_SHAPES:
	.incbin "MAP/MAP_004/SHAPES.table.lz"
	.balign 4, 0
.LField_Map004_GRID:
	.incbin "MAP/MAP_004/GRID.parts.lz"
	.balign 4, 0
.LField_Map004_MARKERS:
	.incbin "MAP/MAP_004/MARKERS.table"
