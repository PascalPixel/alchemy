@ Field_Map072: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map072
Field_Map072:
	map_header 10, 4, 67, 67, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map072, .LField_Map072_METATILES
	map_block Field_Map072, .LField_Map072_SHAPES
	map_block Field_Map072, .LField_Map072_GRID
	map_block Field_Map072, .LField_Map072_ANIMATION
	map_block Field_Map072, .LField_Map072_BLEND
	map_block Field_Map072, .LField_Map072_MARKERS
.LField_Map072_METATILES:
	.incbin "MAP/MAP_072/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map072_SHAPES:
	.incbin "MAP/MAP_072/SHAPES.table.lz"
	.balign 4, 0
.LField_Map072_GRID:
	.incbin "MAP/MAP_072/GRID.parts.lz"
	.balign 4, 0
.LField_Map072_ANIMATION:
	.incbin "MAP/MAP_028/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map072_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map072_MARKERS:
	.incbin "MAP/MAP_072/MARKERS.table"
