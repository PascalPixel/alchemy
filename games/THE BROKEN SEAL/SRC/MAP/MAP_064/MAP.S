@ Field_Map064: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map064
Field_Map064:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map064, .LField_Map064_METATILES
	map_block Field_Map064, .LField_Map064_SHAPES
	map_block Field_Map064, .LField_Map064_GRID
	map_block Field_Map064, .LField_Map064_ANIMATION
	map_block Field_Map064, .LField_Map064_BLEND
	map_block Field_Map064, .LField_Map064_MARKERS
.LField_Map064_METATILES:
	.incbin "MAP/MAP_064/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map064_SHAPES:
	.incbin "MAP/MAP_064/SHAPES.table.lz"
	.balign 4, 0
.LField_Map064_GRID:
	.incbin "MAP/MAP_064/GRID.parts.lz"
	.balign 4, 0
.LField_Map064_ANIMATION:
	.incbin "MAP/MAP_064/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map064_BLEND:
	.incbin "MAP/MAP_064/BLEND.script.lz"
	.balign 4, 0
.LField_Map064_MARKERS:
	.incbin "MAP/MAP_064/MARKERS.table"
