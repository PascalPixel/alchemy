@ Field_Map106: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map106
Field_Map106:
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map106, .LField_Map106_METATILES
	map_block Field_Map106, .LField_Map106_SHAPES
	map_block Field_Map106, .LField_Map106_GRID
	map_block Field_Map106, .LField_Map106_ANIMATION
	map_block Field_Map106
	map_block Field_Map106, .LField_Map106_MARKERS
.LField_Map106_METATILES:
	.incbin "MAP/MAP_106/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map106_SHAPES:
	.incbin "MAP/MAP_106/SHAPES.table.lz"
	.balign 4, 0
.LField_Map106_GRID:
	.incbin "MAP/MAP_106/GRID.parts.lz"
	.balign 4, 0
.LField_Map106_ANIMATION:
	.incbin "MAP/MAP_106/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map106_MARKERS:
	.incbin "MAP/MAP_106/MARKERS.table"
