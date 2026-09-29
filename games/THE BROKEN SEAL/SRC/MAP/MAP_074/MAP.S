@ Field_Map074: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map074
Field_Map074:
	map_header 0, 0, 72, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 72, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 144, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map074, .LField_Map074_METATILES
	map_block Field_Map074, .LField_Map074_SHAPES
	map_block Field_Map074, .LField_Map074_GRID
	map_block Field_Map074, .LField_Map074_ANIMATION
	map_block Field_Map074, .LField_Map074_BLEND
	map_block Field_Map074, .LField_Map074_MARKERS
.LField_Map074_METATILES:
	.incbin "MAP/MAP_074/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map074_SHAPES:
	.incbin "MAP/MAP_074/SHAPES.table.lz"
	.balign 4, 0
.LField_Map074_GRID:
	.incbin "MAP/MAP_074/GRID.parts.lz"
	.balign 4, 0
.LField_Map074_ANIMATION:
	.incbin "MAP/MAP_074/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map074_BLEND:
	.incbin "MAP/MAP_074/BLEND.script.lz"
	.balign 4, 0
.LField_Map074_MARKERS:
	.incbin "MAP/MAP_074/MARKERS.table"
