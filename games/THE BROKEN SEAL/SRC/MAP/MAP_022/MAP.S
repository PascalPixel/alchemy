@ Field_Map022: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map022
Field_Map022:
	map_header 0, 0, 96, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map022, .LField_Map022_METATILES
	map_block Field_Map022, .LField_Map022_SHAPES
	map_block Field_Map022, .LField_Map022_GRID
	map_block Field_Map022, .LField_Map022_ANIMATION
	map_block Field_Map022, .LField_Map022_BLEND
	map_block Field_Map022, .LField_Map022_MARKERS
.LField_Map022_METATILES:
	.incbin "MAP/MAP_022/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map022_SHAPES:
	.incbin "MAP/MAP_022/SHAPES.table.lz"
	.balign 4, 0
.LField_Map022_GRID:
	.incbin "MAP/MAP_022/GRID.parts.lz"
	.balign 4, 0
.LField_Map022_ANIMATION:
	.incbin "MAP/MAP_022/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map022_BLEND:
	.incbin "MAP/MAP_022/BLEND.script.lz"
	.balign 4, 0
.LField_Map022_MARKERS:
	.incbin "MAP/MAP_022/MARKERS.table"
