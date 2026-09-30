@ Field_Map018: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map018
Field_Map018:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map018, .LField_Map018_METATILES
	map_block Field_Map018, .LField_Map018_SHAPES
	map_block Field_Map018, .LField_Map018_GRID
	map_block Field_Map018, .LField_Map018_ANIMATION
	map_block Field_Map018, .LField_Map018_BLEND
	map_block Field_Map018, .LField_Map018_MARKERS
.LField_Map018_METATILES:
	.incbin "MAP/MAP_018/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map018_SHAPES:
	.incbin "MAP/MAP_018/SHAPES.table.lz"
	.balign 4, 0
.LField_Map018_GRID:
	.incbin "MAP/MAP_018/GRID.parts.lz"
	.balign 4, 0
.LField_Map018_ANIMATION:
	.incbin "MAP/MAP_018/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map018_BLEND:
	.incbin "COMMON/MAP/MAP_018/BLEND.script.lz"
	.balign 4, 0
.LField_Map018_MARKERS:
	.incbin "MAP/MAP_018/MARKERS.table"
