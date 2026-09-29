@ Field_Map177: a field map, the same as MAP_001.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map177
Field_Map177:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map177, .LField_Map177_METATILES
	map_block Field_Map177, .LField_Map177_SHAPES
	map_block Field_Map177, .LField_Map177_GRID
	map_block Field_Map177
	map_block Field_Map177
	map_block Field_Map177, .LField_Map177_MARKERS
.LField_Map177_METATILES:
	.incbin "MAP/MAP_001/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map177_SHAPES:
	.incbin "MAP/MAP_001/SHAPES.table.lz"
	.balign 4, 0
.LField_Map177_GRID:
	.incbin "MAP/MAP_001/GRID.parts.lz"
	.balign 4, 0
.LField_Map177_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
