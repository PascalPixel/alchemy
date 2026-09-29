@ Field_Map044: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map044
Field_Map044:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map044, .LField_Map044_METATILES
	map_block Field_Map044, .LField_Map044_SHAPES
	map_block Field_Map044, .LField_Map044_GRID
	map_block Field_Map044, .LField_Map044_ANIMATION
	map_block Field_Map044
	map_block Field_Map044, .LField_Map044_MARKERS
.LField_Map044_METATILES:
	.incbin "MAP/MAP_044/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map044_SHAPES:
	.incbin "MAP/MAP_044/SHAPES.table.lz"
	.balign 4, 0
.LField_Map044_GRID:
	.incbin "MAP/MAP_044/GRID.parts.lz"
	.balign 4, 0
.LField_Map044_ANIMATION:
	.incbin "MAP/MAP_044/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map044_MARKERS:
	.incbin "MAP/MAP_044/MARKERS.table"
