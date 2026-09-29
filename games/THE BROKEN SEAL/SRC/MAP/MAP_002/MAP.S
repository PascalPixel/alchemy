@ Field_Map002: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map002
Field_Map002:
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map002, .LField_Map002_METATILES
	map_block Field_Map002, .LField_Map002_SHAPES
	map_block Field_Map002, .LField_Map002_GRID
	map_block Field_Map002, .LField_Map002_ANIMATION
	map_block Field_Map002
	map_block Field_Map002, .LField_Map002_MARKERS
.LField_Map002_METATILES:
	.incbin "MAP/MAP_002/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map002_SHAPES:
	.incbin "MAP/MAP_002/SHAPES.table.lz"
	.balign 4, 0
.LField_Map002_GRID:
	.incbin "MAP/MAP_002/GRID.parts.lz"
	.balign 4, 0
.LField_Map002_ANIMATION:
	.incbin "MAP/MAP_002/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map002_MARKERS:
	.incbin "MAP/MAP_002/MARKERS.table"
