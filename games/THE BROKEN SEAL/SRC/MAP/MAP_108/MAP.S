@ Field_Map108: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map108
Field_Map108:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map108, .LField_Map108_METATILES
	map_block Field_Map108, .LField_Map108_SHAPES
	map_block Field_Map108, .LField_Map108_GRID
	map_block Field_Map108, .LField_Map108_ANIMATION
	map_block Field_Map108, .LField_Map108_BLEND
	map_block Field_Map108, .LField_Map108_MARKERS
.LField_Map108_METATILES:
	.incbin "MAP/MAP_108/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map108_SHAPES:
	.incbin "MAP/MAP_108/SHAPES.table.lz"
	.balign 4, 0
.LField_Map108_GRID:
	.incbin "MAP/MAP_108/GRID.parts.lz"
	.balign 4, 0
.LField_Map108_ANIMATION:
	.incbin "MAP/MAP_108/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map108_BLEND:
	.incbin "MAP/MAP_108/BLEND.script.lz"
	.balign 4, 0
.LField_Map108_MARKERS:
	.incbin "MAP/MAP_108/MARKERS.table"
