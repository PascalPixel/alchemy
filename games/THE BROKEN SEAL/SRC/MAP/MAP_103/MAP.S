@ Field_Map103: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map103
Field_Map103:
	map_header 0, 0, 45, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, -25, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map103, .LField_Map103_METATILES
	map_block Field_Map103, .LField_Map103_SHAPES
	map_block Field_Map103, .LField_Map103_GRID
	map_block Field_Map103, .LField_Map103_ANIMATION
	map_block Field_Map103
	map_block Field_Map103, .LField_Map103_MARKERS
.LField_Map103_METATILES:
	.incbin "MAP/MAP_103/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map103_SHAPES:
	.incbin "MAP/MAP_103/SHAPES.table.lz"
	.balign 4, 0
.LField_Map103_GRID:
	.incbin "MAP/MAP_103/GRID.parts.lz"
	.balign 4, 0
.LField_Map103_ANIMATION:
	.incbin "MAP/MAP_103/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map103_MARKERS:
	.incbin "MAP/MAP_103/MARKERS.table"
