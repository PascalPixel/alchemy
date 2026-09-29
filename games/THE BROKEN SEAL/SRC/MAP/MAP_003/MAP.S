@ Field_Map003: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map003
Field_Map003:
	map_header 0, 0, 60, 114, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map003, .LField_Map003_METATILES
	map_block Field_Map003, .LField_Map003_SHAPES
	map_block Field_Map003, .LField_Map003_GRID
	map_block Field_Map003, .LField_Map003_ANIMATION
	map_block Field_Map003
	map_block Field_Map003, .LField_Map003_MARKERS
.LField_Map003_METATILES:
	.incbin "MAP/MAP_003/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map003_SHAPES:
	.incbin "MAP/MAP_003/SHAPES.table.lz"
	.balign 4, 0
.LField_Map003_GRID:
	.incbin "MAP/MAP_003/GRID.parts.lz"
	.balign 4, 0
.LField_Map003_ANIMATION:
	.incbin "MAP/MAP_003/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map003_MARKERS:
	.incbin "MAP/MAP_003/MARKERS.table"
