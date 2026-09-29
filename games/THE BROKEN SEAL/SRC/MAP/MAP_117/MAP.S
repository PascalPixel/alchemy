@ Field_Map117: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map117
Field_Map117:
	map_header 0, 0, 45, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map117, .LField_Map117_METATILES
	map_block Field_Map117, .LField_Map117_SHAPES
	map_block Field_Map117, .LField_Map117_GRID
	map_block Field_Map117, .LField_Map117_ANIMATION
	map_block Field_Map117
	map_block Field_Map117, .LField_Map117_MARKERS
.LField_Map117_METATILES:
	.incbin "MAP/MAP_117/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map117_SHAPES:
	.incbin "MAP/MAP_117/SHAPES.table.lz"
	.balign 4, 0
.LField_Map117_GRID:
	.incbin "MAP/MAP_117/GRID.parts.lz"
	.balign 4, 0
.LField_Map117_ANIMATION:
	.incbin "MAP/MAP_117/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map117_MARKERS:
	.incbin "MAP/MAP_117/MARKERS.table"
