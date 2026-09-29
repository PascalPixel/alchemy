@ Field_Map060: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map060
Field_Map060:
	map_header 0, 0, 55, 68, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map060, .LField_Map060_METATILES
	map_block Field_Map060, .LField_Map060_SHAPES
	map_block Field_Map060, .LField_Map060_GRID
	map_block Field_Map060, .LField_Map060_ANIMATION
	map_block Field_Map060, .LField_Map060_BLEND
	map_block Field_Map060, .LField_Map060_MARKERS
.LField_Map060_METATILES:
	.incbin "MAP/MAP_060/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map060_SHAPES:
	.incbin "MAP/MAP_060/SHAPES.table.lz"
	.balign 4, 0
.LField_Map060_GRID:
	.incbin "MAP/MAP_060/GRID.parts.lz"
	.balign 4, 0
.LField_Map060_ANIMATION:
	.incbin "MAP/MAP_060/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map060_BLEND:
	.incbin "MAP/MAP_060/BLEND.script.lz"
	.balign 4, 0
.LField_Map060_MARKERS:
	.incbin "MAP/MAP_060/MARKERS.table"
