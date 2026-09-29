@ Field_Map122: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map122
Field_Map122:
	map_header 0, 0, 48, 56, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 48, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 56, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map122, .LField_Map122_METATILES
	map_block Field_Map122, .LField_Map122_SHAPES
	map_block Field_Map122, .LField_Map122_GRID
	map_block Field_Map122
	map_block Field_Map122
	map_block Field_Map122, .LField_Map122_MARKERS
.LField_Map122_METATILES:
	.incbin "MAP/MAP_122/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map122_SHAPES:
	.incbin "MAP/MAP_122/SHAPES.table.lz"
	.balign 4, 0
.LField_Map122_GRID:
	.incbin "MAP/MAP_122/GRID.parts.lz"
	.balign 4, 0
.LField_Map122_MARKERS:
	.incbin "MAP/MAP_122/MARKERS.table"
