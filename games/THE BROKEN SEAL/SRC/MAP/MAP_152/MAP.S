@ Field_Map152: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map152
Field_Map152:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map152, .LField_Map152_METATILES
	map_block Field_Map152, .LField_Map152_SHAPES
	map_block Field_Map152, .LField_Map152_GRID
	map_block Field_Map152, .LField_Map152_ANIMATION
	map_block Field_Map152, .LField_Map152_BLEND
	map_block Field_Map152, .LField_Map152_MARKERS
.LField_Map152_METATILES:
	.incbin "MAP/MAP_152/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map152_SHAPES:
	.incbin "MAP/MAP_152/SHAPES.table.lz"
	.balign 4, 0
.LField_Map152_GRID:
	.incbin "MAP/MAP_152/GRID.parts.lz"
	.balign 4, 0
.LField_Map152_ANIMATION:
	.incbin "MAP/MAP_152/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map152_BLEND:
	.incbin "MAP/MAP_152/BLEND.script.lz"
	.balign 4, 0
.LField_Map152_MARKERS:
	.incbin "MAP/MAP_152/MARKERS.table"
