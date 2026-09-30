@ Field_Map054: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map054
Field_Map054:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map054, .LField_Map054_METATILES
	map_block Field_Map054, .LField_Map054_SHAPES
	map_block Field_Map054, .LField_Map054_GRID
	map_block Field_Map054, .LField_Map054_ANIMATION
	map_block Field_Map054, .LField_Map054_BLEND
	map_block Field_Map054, .LField_Map054_MARKERS
.LField_Map054_METATILES:
	.incbin "MAP/MAP_054/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map054_SHAPES:
	.incbin "MAP/MAP_054/SHAPES.table.lz"
	.balign 4, 0
.LField_Map054_GRID:
	.incbin "MAP/MAP_054/GRID.parts.lz"
	.balign 4, 0
.LField_Map054_ANIMATION:
	.incbin "MAP/MAP_054/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map054_BLEND:
	.incbin "COMMON/MAP/MAP_054/BLEND.script.lz"
	.balign 4, 0
.LField_Map054_MARKERS:
	.incbin "MAP/MAP_054/MARKERS.table"
