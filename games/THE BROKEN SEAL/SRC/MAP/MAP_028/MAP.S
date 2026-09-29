@ Field_Map028: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map028
Field_Map028:
	map_header 0, 0, 42, 54, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map028, .LField_Map028_METATILES
	map_block Field_Map028, .LField_Map028_SHAPES
	map_block Field_Map028, .LField_Map028_GRID
	map_block Field_Map028, .LField_Map028_ANIMATION
	map_block Field_Map028, .LField_Map028_BLEND
	map_block Field_Map028, .LField_Map028_MARKERS
.LField_Map028_METATILES:
	.incbin "MAP/MAP_028/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map028_SHAPES:
	.incbin "MAP/MAP_028/SHAPES.table.lz"
	.balign 4, 0
.LField_Map028_GRID:
	.incbin "MAP/MAP_028/GRID.parts.lz"
	.balign 4, 0
.LField_Map028_ANIMATION:
	.incbin "MAP/MAP_028/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map028_BLEND:
	.incbin "MAP/MAP_028/BLEND.script.lz"
	.balign 4, 0
.LField_Map028_MARKERS:
	.incbin "MAP/MAP_028/MARKERS.table"
