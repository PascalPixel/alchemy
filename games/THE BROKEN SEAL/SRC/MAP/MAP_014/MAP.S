@ Field_Map014: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map014
Field_Map014:
	map_header 0, 0, 112, 80, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 112, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map014, .LField_Map014_METATILES
	map_block Field_Map014, .LField_Map014_SHAPES
	map_block Field_Map014, .LField_Map014_GRID
	map_block Field_Map014, .LField_Map014_ANIMATION
	map_block Field_Map014, .LField_Map014_BLEND
	map_block Field_Map014, .LField_Map014_MARKERS
.LField_Map014_METATILES:
	.incbin "MAP/MAP_014/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map014_SHAPES:
	.incbin "MAP/MAP_014/SHAPES.table.lz"
	.balign 4, 0
.LField_Map014_GRID:
	.incbin "MAP/MAP_014/GRID.parts.lz"
	.balign 4, 0
.LField_Map014_ANIMATION:
	.incbin "MAP/MAP_014/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map014_BLEND:
	.incbin "MAP/MAP_014/BLEND.script.lz"
	.balign 4, 0
.LField_Map014_MARKERS:
	.incbin "MAP/MAP_014/MARKERS.table"
