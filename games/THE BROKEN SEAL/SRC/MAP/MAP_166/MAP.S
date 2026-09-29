@ Field_Map166: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map166
Field_Map166:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map166, .LField_Map166_METATILES
	map_block Field_Map166, .LField_Map166_SHAPES
	map_block Field_Map166, .LField_Map166_GRID
	map_block Field_Map166, .LField_Map166_ANIMATION
	map_block Field_Map166, .LField_Map166_BLEND
	map_block Field_Map166, .LField_Map166_MARKERS
.LField_Map166_METATILES:
	.incbin "MAP/MAP_166/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map166_SHAPES:
	.incbin "MAP/MAP_166/SHAPES.table.lz"
	.balign 4, 0
.LField_Map166_GRID:
	.incbin "MAP/MAP_166/GRID.parts.lz"
	.balign 4, 0
.LField_Map166_ANIMATION:
	.incbin "MAP/MAP_164/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map166_BLEND:
	.incbin "MAP/MAP_164/BLEND.script.lz"
	.balign 4, 0
.LField_Map166_MARKERS:
	.incbin "MAP/MAP_166/MARKERS.table"
