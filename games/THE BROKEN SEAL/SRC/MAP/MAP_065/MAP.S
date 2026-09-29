@ Field_Map065: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map065
Field_Map065:
	map_header 0, 0, 72, 106, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 106, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map065, .LField_Map065_METATILES
	map_block Field_Map065, .LField_Map065_SHAPES
	map_block Field_Map065, .LField_Map065_GRID
	map_block Field_Map065, .LField_Map065_ANIMATION
	map_block Field_Map065, .LField_Map065_BLEND
	map_block Field_Map065, .LField_Map065_MARKERS
.LField_Map065_METATILES:
	.incbin "MAP/MAP_065/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map065_SHAPES:
	.incbin "MAP/MAP_065/SHAPES.table.lz"
	.balign 4, 0
.LField_Map065_GRID:
	.incbin "MAP/MAP_065/GRID.parts.lz"
	.balign 4, 0
.LField_Map065_ANIMATION:
	.incbin "MAP/MAP_065/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map065_BLEND:
	.incbin "MAP/MAP_065/BLEND.script.lz"
	.balign 4, 0
.LField_Map065_MARKERS:
	.incbin "MAP/MAP_065/MARKERS.table"
