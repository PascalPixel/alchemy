@ Field_Map043: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map043
Field_Map043:
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map043, .LField_Map043_METATILES
	map_block Field_Map043, .LField_Map043_SHAPES
	map_block Field_Map043, .LField_Map043_GRID
	map_block Field_Map043, .LField_Map043_ANIMATION
	map_block Field_Map043, .LField_Map043_BLEND
	map_block Field_Map043, .LField_Map043_MARKERS
.LField_Map043_METATILES:
	.incbin "MAP/MAP_043/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map043_SHAPES:
	.incbin "MAP/MAP_043/SHAPES.table.lz"
	.balign 4, 0
.LField_Map043_GRID:
	.incbin "MAP/MAP_043/GRID.parts.lz"
	.balign 4, 0
.LField_Map043_ANIMATION:
	.incbin "MAP/MAP_043/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map043_BLEND:
	.incbin "MAP/MAP_039/BLEND.script.lz"
	.balign 4, 0
.LField_Map043_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
