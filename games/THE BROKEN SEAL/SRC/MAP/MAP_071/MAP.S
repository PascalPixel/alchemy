@ Field_Map071: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map071
Field_Map071:
	map_header 0, 0, 108, 90, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map071, .LField_Map071_METATILES
	map_block Field_Map071, .LField_Map071_SHAPES
	map_block Field_Map071, .LField_Map071_GRID
	map_block Field_Map071
	map_block Field_Map071
	map_block Field_Map071, .LField_Map071_MARKERS
.LField_Map071_METATILES:
	.incbin "MAP/MAP_071/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map071_SHAPES:
	.incbin "MAP/MAP_042/SHAPES.table.lz"
	.balign 4, 0
.LField_Map071_GRID:
	.incbin "MAP/MAP_071/GRID.parts.lz"
	.balign 4, 0
.LField_Map071_MARKERS:
	.incbin "MAP/MAP_071/MARKERS.table"
