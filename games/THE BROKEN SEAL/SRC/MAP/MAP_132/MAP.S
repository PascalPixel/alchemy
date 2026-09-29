@ Field_Map132: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map132
Field_Map132:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map132, .LField_Map132_METATILES
	map_block Field_Map132, .LField_Map132_SHAPES
	map_block Field_Map132, .LField_Map132_GRID
	map_block Field_Map132, .LField_Map132_ANIMATION
	map_block Field_Map132, .LField_Map132_BLEND
	map_block Field_Map132, .LField_Map132_MARKERS
.LField_Map132_METATILES:
	.incbin "MAP/MAP_132/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map132_SHAPES:
	.incbin "MAP/MAP_130/SHAPES.table.lz"
	.balign 4, 0
.LField_Map132_GRID:
	.incbin "MAP/MAP_132/GRID.parts.lz"
	.balign 4, 0
.LField_Map132_ANIMATION:
	.incbin "MAP/MAP_131/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map132_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map132_MARKERS:
	.incbin "MAP/MAP_132/MARKERS.table"
