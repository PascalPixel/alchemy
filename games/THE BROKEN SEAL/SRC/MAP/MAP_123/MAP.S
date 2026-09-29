@ Field_Map123: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map123
Field_Map123:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map123, .LField_Map123_METATILES
	map_block Field_Map123, .LField_Map123_SHAPES
	map_block Field_Map123, .LField_Map123_GRID
	map_block Field_Map123, .LField_Map123_ANIMATION
	map_block Field_Map123, .LField_Map123_BLEND
	map_block Field_Map123, .LField_Map123_MARKERS
.LField_Map123_METATILES:
	.incbin "MAP/MAP_123/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map123_SHAPES:
	.incbin "MAP/MAP_123/SHAPES.table.lz"
	.balign 4, 0
.LField_Map123_GRID:
	.incbin "MAP/MAP_123/GRID.parts.lz"
	.balign 4, 0
.LField_Map123_ANIMATION:
	.incbin "MAP/MAP_123/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map123_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map123_MARKERS:
	.incbin "MAP/MAP_123/MARKERS.table"
