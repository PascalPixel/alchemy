@ Field_Map009: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map009
Field_Map009:
	map_header 0, 0, 100, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map009, .LField_Map009_METATILES
	map_block Field_Map009, .LField_Map009_SHAPES
	map_block Field_Map009, .LField_Map009_GRID
	map_block Field_Map009, .LField_Map009_ANIMATION
	map_block Field_Map009, .LField_Map009_BLEND
	map_block Field_Map009, .LField_Map009_MARKERS
.LField_Map009_METATILES:
	.incbin "MAP/MAP_009/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map009_SHAPES:
	.incbin "MAP/MAP_009/SHAPES.table.lz"
	.balign 4, 0
.LField_Map009_GRID:
	.incbin "MAP/MAP_009/GRID.parts.lz"
	.balign 4, 0
.LField_Map009_ANIMATION:
	.incbin "MAP/MAP_009/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map009_BLEND:
	.incbin "MAP/MAP_009/BLEND.script.lz"
	.balign 4, 0
.LField_Map009_MARKERS:
	.incbin "MAP/MAP_009/MARKERS.table"
