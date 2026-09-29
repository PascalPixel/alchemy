@ Field_Map146: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map146
Field_Map146:
	map_header 0, 0, 128, 128, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map146, .LField_Map146_METATILES
	map_block Field_Map146, .LField_Map146_SHAPES
	map_block Field_Map146, .LField_Map146_GRID
	map_block Field_Map146, .LField_Map146_ANIMATION
	map_block Field_Map146, .LField_Map146_BLEND
	map_block Field_Map146, .LField_Map146_MARKERS
.LField_Map146_METATILES:
	.incbin "MAP/MAP_146/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map146_SHAPES:
	.incbin "MAP/MAP_146/SHAPES.table.lz"
	.balign 4, 0
.LField_Map146_GRID:
	.incbin "MAP/MAP_146/GRID.parts.lz"
	.balign 4, 0
.LField_Map146_ANIMATION:
	.incbin "MAP/MAP_145/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map146_BLEND:
	.incbin "MAP/MAP_009/BLEND.script.lz"
	.balign 4, 0
.LField_Map146_MARKERS:
	.incbin "MAP/MAP_146/MARKERS.table"
