@ Field_Map001: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map001
Field_Map001:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map001, .LField_Map001_METATILES
	map_block Field_Map001, .LField_Map001_SHAPES
	map_block Field_Map001, .LField_Map001_GRID
	map_block Field_Map001
	map_block Field_Map001
	map_block Field_Map001, .LField_Map001_MARKERS
.LField_Map001_METATILES:
	.incbin "COMMON/MAP/MAP_001/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map001_SHAPES:
	.incbin "COMMON/MAP/MAP_001/SHAPES.table.lz"
	.balign 4, 0
.LField_Map001_GRID:
	.incbin "MAP/MAP_001/GRID.parts.lz"
	.balign 4, 0
.LField_Map001_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
