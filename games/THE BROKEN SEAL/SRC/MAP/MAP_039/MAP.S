@ Field_Map039: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map039
Field_Map039:
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map039, .LField_Map039_METATILES
	map_block Field_Map039, .LField_Map039_SHAPES
	map_block Field_Map039, .LField_Map039_GRID
	map_block Field_Map039, .LField_Map039_ANIMATION
	map_block Field_Map039, .LField_Map039_BLEND
	map_block Field_Map039, .LField_Map039_MARKERS
.LField_Map039_METATILES:
	.incbin "MAP/MAP_039/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map039_SHAPES:
	.incbin "MAP/MAP_039/SHAPES.table.lz"
	.balign 4, 0
.LField_Map039_GRID:
	.incbin "MAP/MAP_039/GRID.parts.lz"
	.balign 4, 0
.LField_Map039_ANIMATION:
	.incbin "MAP/MAP_039/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map039_BLEND:
	.incbin "MAP/MAP_039/BLEND.script.lz"
	.balign 4, 0
.LField_Map039_MARKERS:
	.incbin "MAP/MAP_039/MARKERS.table"
