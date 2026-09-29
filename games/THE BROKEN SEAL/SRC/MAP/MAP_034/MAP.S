@ Field_Map034: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map034
Field_Map034:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map034, .LField_Map034_METATILES
	map_block Field_Map034, .LField_Map034_SHAPES
	map_block Field_Map034, .LField_Map034_GRID
	map_block Field_Map034, .LField_Map034_ANIMATION
	map_block Field_Map034
	map_block Field_Map034, .LField_Map034_MARKERS
.LField_Map034_METATILES:
	.incbin "MAP/MAP_034/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map034_SHAPES:
	.incbin "MAP/MAP_034/SHAPES.table.lz"
	.balign 4, 0
.LField_Map034_GRID:
	.incbin "MAP/MAP_034/GRID.parts.lz"
	.balign 4, 0
.LField_Map034_ANIMATION:
	.incbin "MAP/MAP_034/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map034_MARKERS:
	.incbin "MAP/MAP_034/MARKERS.table"
