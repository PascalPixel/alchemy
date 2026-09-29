@ Field_Map052: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map052
Field_Map052:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map052, .LField_Map052_METATILES
	map_block Field_Map052, .LField_Map052_SHAPES
	map_block Field_Map052, .LField_Map052_GRID
	map_block Field_Map052, .LField_Map052_ANIMATION
	map_block Field_Map052, .LField_Map052_BLEND
	map_block Field_Map052, .LField_Map052_MARKERS
.LField_Map052_METATILES:
	.incbin "MAP/MAP_052/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map052_SHAPES:
	.incbin "MAP/MAP_052/SHAPES.table.lz"
	.balign 4, 0
.LField_Map052_GRID:
	.incbin "MAP/MAP_052/GRID.parts.lz"
	.balign 4, 0
.LField_Map052_ANIMATION:
	.incbin "MAP/MAP_052/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map052_BLEND:
	.incbin "MAP/MAP_018/BLEND.script.lz"
	.balign 4, 0
.LField_Map052_MARKERS:
	.incbin "MAP/MAP_052/MARKERS.table"
