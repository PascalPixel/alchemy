@ Field_Map179: a field map, the same as MAP_001.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map179
Field_Map179:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map179, .LField_Map179_METATILES
	map_block Field_Map179, .LField_Map179_SHAPES
	map_block Field_Map179, .LField_Map179_GRID
	map_block Field_Map179
	map_block Field_Map179
	map_block Field_Map179, .LField_Map179_MARKERS
.LField_Map179_METATILES:
	.incbin "MAP/MAP_001/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map179_SHAPES:
	.incbin "MAP/MAP_001/SHAPES.table.lz"
	.balign 4, 0
.LField_Map179_GRID:
	.incbin "MAP/MAP_001/GRID.parts.lz"
	.balign 4, 0
.LField_Map179_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
