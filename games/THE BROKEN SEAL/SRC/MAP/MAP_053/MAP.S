@ Field_Map053: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map053
Field_Map053:
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 70, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map053, .LField_Map053_METATILES
	map_block Field_Map053, .LField_Map053_SHAPES
	map_block Field_Map053, .LField_Map053_GRID
	map_block Field_Map053, .LField_Map053_ANIMATION
	map_block Field_Map053, .LField_Map053_BLEND
	map_block Field_Map053, .LField_Map053_MARKERS
.LField_Map053_METATILES:
	.incbin "MAP/MAP_053/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map053_SHAPES:
	.incbin "MAP/MAP_053/SHAPES.table.lz"
	.balign 4, 0
.LField_Map053_GRID:
	.incbin "MAP/MAP_053/GRID.parts.lz"
	.balign 4, 0
.LField_Map053_ANIMATION:
	.incbin "MAP/MAP_053/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map053_BLEND:
	.incbin "MAP/MAP_053/BLEND.script.lz"
	.balign 4, 0
.LField_Map053_MARKERS:
	.incbin "MAP/MAP_053/MARKERS.table"
