@ Field_Map102: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map102
Field_Map102:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map102, .LField_Map102_METATILES
	map_block Field_Map102, .LField_Map102_SHAPES
	map_block Field_Map102, .LField_Map102_GRID
	map_block Field_Map102, .LField_Map102_ANIMATION
	map_block Field_Map102, .LField_Map102_BLEND
	map_block Field_Map102, .LField_Map102_MARKERS
.LField_Map102_METATILES:
	.incbin "MAP/MAP_102/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map102_SHAPES:
	.incbin "MAP/MAP_102/SHAPES.table.lz"
	.balign 4, 0
.LField_Map102_GRID:
	.incbin "MAP/MAP_102/GRID.parts.lz"
	.balign 4, 0
.LField_Map102_ANIMATION:
	.incbin "MAP/MAP_102/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map102_BLEND:
	.incbin "MAP/MAP_102/BLEND.script.lz"
	.balign 4, 0
.LField_Map102_MARKERS:
	.incbin "MAP/MAP_102/MARKERS.table"
