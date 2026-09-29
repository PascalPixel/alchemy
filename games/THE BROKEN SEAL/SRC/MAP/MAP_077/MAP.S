@ Field_Map077: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map077
Field_Map077:
	map_header 0, 0, 64, 68, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map077, .LField_Map077_METATILES
	map_block Field_Map077, .LField_Map077_SHAPES
	map_block Field_Map077, .LField_Map077_GRID
	map_block Field_Map077, .LField_Map077_ANIMATION
	map_block Field_Map077, .LField_Map077_BLEND
	map_block Field_Map077, .LField_Map077_MARKERS
.LField_Map077_METATILES:
	.incbin "MAP/MAP_077/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map077_SHAPES:
	.incbin "MAP/MAP_077/SHAPES.table.lz"
	.balign 4, 0
.LField_Map077_GRID:
	.incbin "MAP/MAP_077/GRID.parts.lz"
	.balign 4, 0
.LField_Map077_ANIMATION:
	.incbin "MAP/MAP_077/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map077_BLEND:
	.incbin "MAP/MAP_077/BLEND.script.lz"
	.balign 4, 0
.LField_Map077_MARKERS:
	.incbin "MAP/MAP_077/MARKERS.table"
