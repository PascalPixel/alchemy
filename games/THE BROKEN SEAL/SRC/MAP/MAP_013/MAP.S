@ Field_Map013: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map013
Field_Map013:
	map_header 0, 0, 98, 88, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 88, 16, 16, 0, 0, 0xff, 0xff
	map_layer 98, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map013, .LField_Map013_METATILES
	map_block Field_Map013, .LField_Map013_SHAPES
	map_block Field_Map013, .LField_Map013_GRID
	map_block Field_Map013, .LField_Map013_ANIMATION
	map_block Field_Map013, .LField_Map013_BLEND
	map_block Field_Map013, .LField_Map013_MARKERS
.LField_Map013_METATILES:
	.incbin "MAP/MAP_013/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map013_SHAPES:
	.incbin "MAP/MAP_001/SHAPES.table.lz"
	.balign 4, 0
.LField_Map013_GRID:
	.incbin "MAP/MAP_013/GRID.parts.lz"
	.balign 4, 0
.LField_Map013_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map013_BLEND:
	.incbin "MAP/MAP_012/BLEND.script.lz"
	.balign 4, 0
.LField_Map013_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
