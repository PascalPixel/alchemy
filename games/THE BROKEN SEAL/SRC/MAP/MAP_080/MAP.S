@ Field_Map080: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map080
Field_Map080:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map080, .LField_Map080_METATILES
	map_block Field_Map080, .LField_Map080_SHAPES
	map_block Field_Map080, .LField_Map080_GRID
	map_block Field_Map080, .LField_Map080_ANIMATION
	map_block Field_Map080, .LField_Map080_BLEND
	map_block Field_Map080, .LField_Map080_MARKERS
.LField_Map080_METATILES:
	.incbin "MAP/MAP_080/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map080_SHAPES:
	.incbin "MAP/MAP_080/SHAPES.table.lz"
	.balign 4, 0
.LField_Map080_GRID:
	.incbin "MAP/MAP_080/GRID.parts.lz"
	.balign 4, 0
.LField_Map080_ANIMATION:
	.incbin "MAP/MAP_080/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map080_BLEND:
	.incbin "MAP/MAP_080/BLEND.script.lz"
	.balign 4, 0
.LField_Map080_MARKERS:
	.incbin "MAP/MAP_080/MARKERS.table"
