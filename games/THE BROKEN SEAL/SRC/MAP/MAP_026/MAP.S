@ Field_Map026: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map026
Field_Map026:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map026, .LField_Map026_METATILES
	map_block Field_Map026, .LField_Map026_SHAPES
	map_block Field_Map026, .LField_Map026_GRID
	map_block Field_Map026, .LField_Map026_ANIMATION
	map_block Field_Map026, .LField_Map026_BLEND
	map_block Field_Map026, .LField_Map026_MARKERS
.LField_Map026_METATILES:
	.incbin "MAP/MAP_026/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map026_SHAPES:
	.incbin "MAP/MAP_026/SHAPES.table.lz"
	.balign 4, 0
.LField_Map026_GRID:
	.incbin "MAP/MAP_026/GRID.parts.lz"
	.balign 4, 0
.LField_Map026_ANIMATION:
	.incbin "MAP/MAP_025/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map026_BLEND:
	.incbin "MAP/MAP_025/BLEND.script.lz"
	.balign 4, 0
.LField_Map026_MARKERS:
	.incbin "MAP/MAP_026/MARKERS.table"
