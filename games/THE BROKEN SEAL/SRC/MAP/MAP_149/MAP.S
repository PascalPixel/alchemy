@ Field_Map149: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map149
Field_Map149:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map149, .LField_Map149_METATILES
	map_block Field_Map149, .LField_Map149_SHAPES
	map_block Field_Map149, .LField_Map149_GRID
	map_block Field_Map149, .LField_Map149_ANIMATION
	map_block Field_Map149, .LField_Map149_BLEND
	map_block Field_Map149, .LField_Map149_MARKERS
.LField_Map149_METATILES:
	.incbin "MAP/MAP_149/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map149_SHAPES:
	.incbin "MAP/MAP_130/SHAPES.table.lz"
	.balign 4, 0
.LField_Map149_GRID:
	.incbin "MAP/MAP_149/GRID.parts.lz"
	.balign 4, 0
.LField_Map149_ANIMATION:
	.incbin "MAP/MAP_080/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map149_BLEND:
	.incbin "MAP/MAP_025/BLEND.script.lz"
	.balign 4, 0
.LField_Map149_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
