@ Field_Map038: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map038
Field_Map038:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map038, .LField_Map038_METATILES
	map_block Field_Map038, .LField_Map038_SHAPES
	map_block Field_Map038, .LField_Map038_GRID
	map_block Field_Map038, .LField_Map038_ANIMATION
	map_block Field_Map038, .LField_Map038_BLEND
	map_block Field_Map038, .LField_Map038_MARKERS
.LField_Map038_METATILES:
	.incbin "MAP/MAP_038/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map038_SHAPES:
	.incbin "MAP/MAP_038/SHAPES.table.lz"
	.balign 4, 0
.LField_Map038_GRID:
	.incbin "MAP/MAP_038/GRID.parts.lz"
	.balign 4, 0
.LField_Map038_ANIMATION:
	.incbin "MAP/MAP_038/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map038_BLEND:
	.incbin "MAP/MAP_038/BLEND.script.lz"
	.balign 4, 0
.LField_Map038_MARKERS:
	.incbin "MAP/MAP_038/MARKERS.table"
