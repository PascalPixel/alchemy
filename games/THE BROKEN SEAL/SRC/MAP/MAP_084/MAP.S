@ Field_Map084: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map084
Field_Map084:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map084, .LField_Map084_METATILES
	map_block Field_Map084, .LField_Map084_SHAPES
	map_block Field_Map084, .LField_Map084_GRID
	map_block Field_Map084, .LField_Map084_ANIMATION
	map_block Field_Map084, .LField_Map084_BLEND
	map_block Field_Map084, .LField_Map084_MARKERS
.LField_Map084_METATILES:
	.incbin "MAP/MAP_084/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map084_SHAPES:
	.incbin "MAP/MAP_084/SHAPES.table.lz"
	.balign 4, 0
.LField_Map084_GRID:
	.incbin "MAP/MAP_084/GRID.parts.lz"
	.balign 4, 0
.LField_Map084_ANIMATION:
	.incbin "MAP/MAP_084/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map084_BLEND:
	.incbin "MAP/MAP_084/BLEND.script.lz"
	.balign 4, 0
.LField_Map084_MARKERS:
	.incbin "MAP/MAP_084/MARKERS.table"
