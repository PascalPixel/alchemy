@ Field_Map066: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map066
Field_Map066:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map066, .LField_Map066_METATILES
	map_block Field_Map066, .LField_Map066_SHAPES
	map_block Field_Map066, .LField_Map066_GRID
	map_block Field_Map066, .LField_Map066_ANIMATION
	map_block Field_Map066, .LField_Map066_BLEND
	map_block Field_Map066, .LField_Map066_MARKERS
.LField_Map066_METATILES:
	.incbin "MAP/MAP_066/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map066_SHAPES:
	.incbin "MAP/MAP_066/SHAPES.table.lz"
	.balign 4, 0
.LField_Map066_GRID:
	.incbin "MAP/MAP_066/GRID.parts.lz"
	.balign 4, 0
.LField_Map066_ANIMATION:
	.incbin "MAP/MAP_033/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map066_BLEND:
	.incbin "MAP/MAP_063/BLEND.script.lz"
	.balign 4, 0
.LField_Map066_MARKERS:
	.incbin "MAP/MAP_066/MARKERS.table"
