@ Field_Map097: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map097
Field_Map097:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map097, .LField_Map097_METATILES
	map_block Field_Map097, .LField_Map097_SHAPES
	map_block Field_Map097, .LField_Map097_GRID
	map_block Field_Map097, .LField_Map097_ANIMATION
	map_block Field_Map097, .LField_Map097_BLEND
	map_block Field_Map097, .LField_Map097_MARKERS
.LField_Map097_METATILES:
	.incbin "MAP/MAP_097/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map097_SHAPES:
	.incbin "MAP/MAP_097/SHAPES.table.lz"
	.balign 4, 0
.LField_Map097_GRID:
	.incbin "MAP/MAP_097/GRID.parts.lz"
	.balign 4, 0
.LField_Map097_ANIMATION:
	.incbin "MAP/MAP_097/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map097_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map097_MARKERS:
	.incbin "MAP/MAP_097/MARKERS.table"
