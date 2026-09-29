@ Field_Map036: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map036
Field_Map036:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map036, .LField_Map036_METATILES
	map_block Field_Map036, .LField_Map036_SHAPES
	map_block Field_Map036, .LField_Map036_GRID
	map_block Field_Map036, .LField_Map036_ANIMATION
	map_block Field_Map036, .LField_Map036_BLEND
	map_block Field_Map036, .LField_Map036_MARKERS
.LField_Map036_METATILES:
	.incbin "MAP/MAP_036/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map036_SHAPES:
	.incbin "MAP/MAP_036/SHAPES.table.lz"
	.balign 4, 0
.LField_Map036_GRID:
	.incbin "MAP/MAP_036/GRID.parts.lz"
	.balign 4, 0
.LField_Map036_ANIMATION:
	.incbin "MAP/MAP_036/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map036_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map036_MARKERS:
	.incbin "MAP/MAP_036/MARKERS.table"
