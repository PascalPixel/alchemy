@ Field_Map138: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map138
Field_Map138:
	map_header 0, 0, 56, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map138, .LField_Map138_METATILES
	map_block Field_Map138, .LField_Map138_SHAPES
	map_block Field_Map138, .LField_Map138_GRID
	map_block Field_Map138, .LField_Map138_ANIMATION
	map_block Field_Map138, .LField_Map138_BLEND
	map_block Field_Map138, .LField_Map138_MARKERS
.LField_Map138_METATILES:
	.incbin "MAP/MAP_138/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map138_SHAPES:
	.incbin "MAP/MAP_138/SHAPES.table.lz"
	.balign 4, 0
.LField_Map138_GRID:
	.incbin "MAP/MAP_138/GRID.parts.lz"
	.balign 4, 0
.LField_Map138_ANIMATION:
	.incbin "MAP/MAP_138/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map138_BLEND:
	.incbin "MAP/MAP_138/BLEND.script.lz"
	.balign 4, 0
.LField_Map138_MARKERS:
	.incbin "MAP/MAP_138/MARKERS.table"
