@ Field_Map090: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map090
Field_Map090:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map090, .LField_Map090_METATILES
	map_block Field_Map090, .LField_Map090_SHAPES
	map_block Field_Map090, .LField_Map090_GRID
	map_block Field_Map090, .LField_Map090_ANIMATION
	map_block Field_Map090
	map_block Field_Map090, .LField_Map090_MARKERS
.LField_Map090_METATILES:
	.incbin "MAP/MAP_090/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map090_SHAPES:
	.incbin "MAP/MAP_090/SHAPES.table.lz"
	.balign 4, 0
.LField_Map090_GRID:
	.incbin "MAP/MAP_090/GRID.parts.lz"
	.balign 4, 0
.LField_Map090_ANIMATION:
	.incbin "MAP/MAP_090/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map090_MARKERS:
	.incbin "MAP/MAP_090/MARKERS.table"
