@ Field_Map100: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map100
Field_Map100:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map100, .LField_Map100_METATILES
	map_block Field_Map100, .LField_Map100_SHAPES
	map_block Field_Map100, .LField_Map100_GRID
	map_block Field_Map100, .LField_Map100_ANIMATION
	map_block Field_Map100, .LField_Map100_BLEND
	map_block Field_Map100, .LField_Map100_MARKERS
.LField_Map100_METATILES:
	.incbin "MAP/MAP_100/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map100_SHAPES:
	.incbin "MAP/MAP_100/SHAPES.table.lz"
	.balign 4, 0
.LField_Map100_GRID:
	.incbin "MAP/MAP_100/GRID.parts.lz"
	.balign 4, 0
.LField_Map100_ANIMATION:
	.incbin "MAP/MAP_033/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map100_BLEND:
	.incbin "MAP/MAP_063/BLEND.script.lz"
	.balign 4, 0
.LField_Map100_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
