@ Field_Map098: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map098
Field_Map098:
	map_header 0, 0, 56, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map098, .LField_Map098_METATILES
	map_block Field_Map098, .LField_Map098_SHAPES
	map_block Field_Map098, .LField_Map098_GRID
	map_block Field_Map098, .LField_Map098_ANIMATION
	map_block Field_Map098, .LField_Map098_BLEND
	map_block Field_Map098, .LField_Map098_MARKERS
.LField_Map098_METATILES:
	.incbin "MAP/MAP_098/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map098_SHAPES:
	.incbin "MAP/MAP_098/SHAPES.table.lz"
	.balign 4, 0
.LField_Map098_GRID:
	.incbin "MAP/MAP_098/GRID.parts.lz"
	.balign 4, 0
.LField_Map098_ANIMATION:
	.incbin "MAP/MAP_098/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map098_BLEND:
	.incbin "MAP/MAP_098/BLEND.script.lz"
	.balign 4, 0
.LField_Map098_MARKERS:
	.incbin "MAP/MAP_098/MARKERS.table"
