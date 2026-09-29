@ Field_Map131: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map131
Field_Map131:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map131, .LField_Map131_METATILES
	map_block Field_Map131, .LField_Map131_SHAPES
	map_block Field_Map131, .LField_Map131_GRID
	map_block Field_Map131, .LField_Map131_ANIMATION
	map_block Field_Map131, .LField_Map131_BLEND
	map_block Field_Map131, .LField_Map131_MARKERS
.LField_Map131_METATILES:
	.incbin "MAP/MAP_131/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map131_SHAPES:
	.incbin "MAP/MAP_131/SHAPES.table.lz"
	.balign 4, 0
.LField_Map131_GRID:
	.incbin "MAP/MAP_131/GRID.parts.lz"
	.balign 4, 0
.LField_Map131_ANIMATION:
	.incbin "MAP/MAP_131/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map131_BLEND:
	.incbin "MAP/MAP_131/BLEND.script.lz"
	.balign 4, 0
.LField_Map131_MARKERS:
	.incbin "MAP/MAP_131/MARKERS.table"
