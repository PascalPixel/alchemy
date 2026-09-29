@ Field_Map129: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map129
Field_Map129:
	map_header 0, 0, 206, 52, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 52, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 104, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map129, .LField_Map129_METATILES
	map_block Field_Map129, .LField_Map129_SHAPES
	map_block Field_Map129, .LField_Map129_GRID
	map_block Field_Map129, .LField_Map129_ANIMATION
	map_block Field_Map129, .LField_Map129_BLEND
	map_block Field_Map129, .LField_Map129_MARKERS
.LField_Map129_METATILES:
	.incbin "MAP/MAP_129/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map129_SHAPES:
	.incbin "MAP/MAP_129/SHAPES.table.lz"
	.balign 4, 0
.LField_Map129_GRID:
	.incbin "MAP/MAP_129/GRID.parts.lz"
	.balign 4, 0
.LField_Map129_ANIMATION:
	.incbin "MAP/MAP_129/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map129_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map129_MARKERS:
	.incbin "MAP/MAP_129/MARKERS.table"
