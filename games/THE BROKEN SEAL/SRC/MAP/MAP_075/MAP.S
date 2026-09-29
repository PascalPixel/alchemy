@ Field_Map075: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map075
Field_Map075:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map075, .LField_Map075_METATILES
	map_block Field_Map075, .LField_Map075_SHAPES
	map_block Field_Map075, .LField_Map075_GRID
	map_block Field_Map075, .LField_Map075_ANIMATION
	map_block Field_Map075, .LField_Map075_BLEND
	map_block Field_Map075, .LField_Map075_MARKERS
.LField_Map075_METATILES:
	.incbin "MAP/MAP_075/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map075_SHAPES:
	.incbin "MAP/MAP_075/SHAPES.table.lz"
	.balign 4, 0
.LField_Map075_GRID:
	.incbin "MAP/MAP_075/GRID.parts.lz"
	.balign 4, 0
.LField_Map075_ANIMATION:
	.incbin "MAP/MAP_075/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map075_BLEND:
	.incbin "MAP/MAP_075/BLEND.script.lz"
	.balign 4, 0
.LField_Map075_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
