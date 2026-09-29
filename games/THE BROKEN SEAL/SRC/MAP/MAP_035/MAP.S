@ Field_Map035: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map035
Field_Map035:
	map_header 0, 0, 64, 48, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 48, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map035, .LField_Map035_METATILES
	map_block Field_Map035, .LField_Map035_SHAPES
	map_block Field_Map035, .LField_Map035_GRID
	map_block Field_Map035
	map_block Field_Map035
	map_block Field_Map035, .LField_Map035_MARKERS
.LField_Map035_METATILES:
	.incbin "MAP/MAP_035/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map035_SHAPES:
	.incbin "MAP/MAP_035/SHAPES.table.lz"
	.balign 4, 0
.LField_Map035_GRID:
	.incbin "MAP/MAP_035/GRID.parts.lz"
	.balign 4, 0
.LField_Map035_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
