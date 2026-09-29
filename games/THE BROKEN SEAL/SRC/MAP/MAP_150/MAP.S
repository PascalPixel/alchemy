@ Field_Map150: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map150
Field_Map150:
	map_header 0, 0, 48, 48, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map150, .LField_Map150_METATILES
	map_block Field_Map150, .LField_Map150_SHAPES
	map_block Field_Map150, .LField_Map150_GRID
	map_block Field_Map150, .LField_Map150_ANIMATION
	map_block Field_Map150
	map_block Field_Map150, .LField_Map150_MARKERS
.LField_Map150_METATILES:
	.incbin "MAP/MAP_150/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map150_SHAPES:
	.incbin "MAP/MAP_150/SHAPES.table.lz"
	.balign 4, 0
.LField_Map150_GRID:
	.incbin "MAP/MAP_150/GRID.parts.lz"
	.balign 4, 0
.LField_Map150_ANIMATION:
	.incbin "MAP/MAP_150/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map150_MARKERS:
	.incbin "MAP/MAP_150/MARKERS.table"
