@ Field_Map155: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map155
Field_Map155:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map155, .LField_Map155_METATILES
	map_block Field_Map155, .LField_Map155_SHAPES
	map_block Field_Map155, .LField_Map155_GRID
	map_block Field_Map155, .LField_Map155_ANIMATION
	map_block Field_Map155, .LField_Map155_BLEND
	map_block Field_Map155, .LField_Map155_MARKERS
.LField_Map155_METATILES:
	.incbin "MAP/MAP_155/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map155_SHAPES:
	.incbin "MAP/MAP_155/SHAPES.table.lz"
	.balign 4, 0
.LField_Map155_GRID:
	.incbin "MAP/MAP_155/GRID.parts.lz"
	.balign 4, 0
.LField_Map155_ANIMATION:
	.incbin "MAP/MAP_155/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map155_BLEND:
	.incbin "MAP/MAP_155/BLEND.script.lz"
	.balign 4, 0
.LField_Map155_MARKERS:
	.incbin "MAP/MAP_155/MARKERS.table"
