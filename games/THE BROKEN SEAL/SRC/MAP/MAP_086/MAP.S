@ Field_Map086: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map086
Field_Map086:
	map_header 0, 0, 100, 68, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map086, .LField_Map086_METATILES
	map_block Field_Map086, .LField_Map086_SHAPES
	map_block Field_Map086, .LField_Map086_GRID
	map_block Field_Map086, .LField_Map086_ANIMATION
	map_block Field_Map086, .LField_Map086_BLEND
	map_block Field_Map086, .LField_Map086_MARKERS
.LField_Map086_METATILES:
	.incbin "MAP/MAP_086/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map086_SHAPES:
	.incbin "MAP/MAP_086/SHAPES.table.lz"
	.balign 4, 0
.LField_Map086_GRID:
	.incbin "MAP/MAP_086/GRID.parts.lz"
	.balign 4, 0
.LField_Map086_ANIMATION:
	.incbin "MAP/MAP_086/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map086_BLEND:
	.incbin "MAP/MAP_086/BLEND.script.lz"
	.balign 4, 0
.LField_Map086_MARKERS:
	.incbin "MAP/MAP_086/MARKERS.table"
