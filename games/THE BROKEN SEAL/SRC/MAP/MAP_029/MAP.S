@ Field_Map029: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map029
Field_Map029:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map029, .LField_Map029_METATILES
	map_block Field_Map029, .LField_Map029_SHAPES
	map_block Field_Map029, .LField_Map029_GRID
	map_block Field_Map029, .LField_Map029_ANIMATION
	map_block Field_Map029, .LField_Map029_BLEND
	map_block Field_Map029, .LField_Map029_MARKERS
.LField_Map029_METATILES:
	.incbin "MAP/MAP_029/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map029_SHAPES:
	.incbin "MAP/MAP_029/SHAPES.table.lz"
	.balign 4, 0
.LField_Map029_GRID:
	.incbin "MAP/MAP_029/GRID.parts.lz"
	.balign 4, 0
.LField_Map029_ANIMATION:
	.incbin "COMMON/MAP/MAP_029/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map029_BLEND:
	.incbin "MAP/MAP_029/BLEND.script.lz"
	.balign 4, 0
.LField_Map029_MARKERS:
	.incbin "MAP/MAP_029/MARKERS.table"
