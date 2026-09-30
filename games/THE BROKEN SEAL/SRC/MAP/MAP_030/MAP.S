@ Field_Map030: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map030
Field_Map030:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map030, .LField_Map030_METATILES
	map_block Field_Map030, .LField_Map030_SHAPES
	map_block Field_Map030, .LField_Map030_GRID
	map_block Field_Map030, .LField_Map030_ANIMATION
	map_block Field_Map030
	map_block Field_Map030, .LField_Map030_MARKERS
.LField_Map030_METATILES:
	.incbin "MAP/MAP_030/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map030_SHAPES:
	.incbin "MAP/MAP_030/SHAPES.table.lz"
	.balign 4, 0
.LField_Map030_GRID:
	.incbin "MAP/MAP_030/GRID.parts.lz"
	.balign 4, 0
.LField_Map030_ANIMATION:
	.incbin "COMMON/MAP/MAP_029/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map030_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
