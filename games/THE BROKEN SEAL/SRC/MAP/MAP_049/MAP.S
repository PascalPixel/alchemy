@ Field_Map049: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map049
Field_Map049:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map049, .LField_Map049_METATILES
	map_block Field_Map049, .LField_Map049_SHAPES
	map_block Field_Map049, .LField_Map049_GRID
	map_block Field_Map049, .LField_Map049_ANIMATION
	map_block Field_Map049
	map_block Field_Map049, .LField_Map049_MARKERS
.LField_Map049_METATILES:
	.incbin "MAP/MAP_049/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map049_SHAPES:
	.incbin "MAP/MAP_049/SHAPES.table.lz"
	.balign 4, 0
.LField_Map049_GRID:
	.incbin "MAP/MAP_049/GRID.parts.lz"
	.balign 4, 0
.LField_Map049_ANIMATION:
	.incbin "MAP/MAP_033/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map049_MARKERS:
	.incbin "MAP/MAP_049/MARKERS.table"
