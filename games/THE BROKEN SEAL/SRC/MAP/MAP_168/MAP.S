@ Field_Map168: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map168
Field_Map168:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map168, .LField_Map168_METATILES
	map_block Field_Map168, .LField_Map168_SHAPES
	map_block Field_Map168, .LField_Map168_GRID
	map_block Field_Map168, .LField_Map168_ANIMATION
	map_block Field_Map168
	map_block Field_Map168, .LField_Map168_MARKERS
.LField_Map168_METATILES:
	.incbin "MAP/MAP_168/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map168_SHAPES:
	.incbin "MAP/MAP_168/SHAPES.table.lz"
	.balign 4, 0
.LField_Map168_GRID:
	.incbin "MAP/MAP_168/GRID.parts.lz"
	.balign 4, 0
.LField_Map168_ANIMATION:
	.incbin "MAP/MAP_054/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map168_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
