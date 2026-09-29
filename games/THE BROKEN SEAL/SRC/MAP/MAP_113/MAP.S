@ Field_Map113: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map113
Field_Map113:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map113, .LField_Map113_METATILES
	map_block Field_Map113, .LField_Map113_SHAPES
	map_block Field_Map113, .LField_Map113_GRID
	map_block Field_Map113, .LField_Map113_ANIMATION
	map_block Field_Map113, .LField_Map113_BLEND
	map_block Field_Map113, .LField_Map113_MARKERS
.LField_Map113_METATILES:
	.incbin "MAP/MAP_113/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map113_SHAPES:
	.incbin "MAP/MAP_113/SHAPES.table.lz"
	.balign 4, 0
.LField_Map113_GRID:
	.incbin "MAP/MAP_113/GRID.parts.lz"
	.balign 4, 0
.LField_Map113_ANIMATION:
	.incbin "MAP/MAP_113/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map113_BLEND:
	.incbin "MAP/MAP_113/BLEND.script.lz"
	.balign 4, 0
.LField_Map113_MARKERS:
	.incbin "MAP/MAP_113/MARKERS.table"
