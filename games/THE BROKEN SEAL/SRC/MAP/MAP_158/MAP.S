@ Field_Map158: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map158
Field_Map158:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map158, .LField_Map158_METATILES
	map_block Field_Map158, .LField_Map158_SHAPES
	map_block Field_Map158, .LField_Map158_GRID
	map_block Field_Map158, .LField_Map158_ANIMATION
	map_block Field_Map158
	map_block Field_Map158, .LField_Map158_MARKERS
.LField_Map158_METATILES:
	.incbin "MAP/MAP_158/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map158_SHAPES:
	.incbin "MAP/MAP_158/SHAPES.table.lz"
	.balign 4, 0
.LField_Map158_GRID:
	.incbin "MAP/MAP_158/GRID.parts.lz"
	.balign 4, 0
.LField_Map158_ANIMATION:
	.incbin "MAP/MAP_033/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map158_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
