@ Field_Map148: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map148
Field_Map148:
	map_header 0, 0, 64, 192, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 192, 0, 20, 20, -121, 35, 0x1f, 0x1f
	map_block Field_Map148, .LField_Map148_METATILES
	map_block Field_Map148, .LField_Map148_SHAPES
	map_block Field_Map148, .LField_Map148_GRID
	map_block Field_Map148
	map_block Field_Map148
	map_block Field_Map148, .LField_Map148_MARKERS
.LField_Map148_METATILES:
	.incbin "MAP/MAP_148/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map148_SHAPES:
	.incbin "MAP/MAP_148/SHAPES.table.lz"
	.balign 4, 0
.LField_Map148_GRID:
	.incbin "MAP/MAP_148/GRID.parts.lz"
	.balign 4, 0
.LField_Map148_MARKERS:
	.incbin "MAP/MAP_148/MARKERS.table"
