@ Field_Map125: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map125
Field_Map125:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map125, .LField_Map125_METATILES
	map_block Field_Map125, .LField_Map125_SHAPES
	map_block Field_Map125, .LField_Map125_GRID
	map_block Field_Map125, .LField_Map125_ANIMATION
	map_block Field_Map125, .LField_Map125_BLEND
	map_block Field_Map125, .LField_Map125_MARKERS
.LField_Map125_METATILES:
	.incbin "MAP/MAP_125/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map125_SHAPES:
	.incbin "MAP/MAP_125/SHAPES.table.lz"
	.balign 4, 0
.LField_Map125_GRID:
	.incbin "MAP/MAP_125/GRID.parts.lz"
	.balign 4, 0
.LField_Map125_ANIMATION:
	.incbin "MAP/MAP_125/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map125_BLEND:
	.incbin "MAP/MAP_125/BLEND.script.lz"
	.balign 4, 0
.LField_Map125_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
