@ Field_Map107: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map107
Field_Map107:
	map_header 0, 0, 64, 78, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 76, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 152, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map107, .LField_Map107_METATILES
	map_block Field_Map107, .LField_Map107_SHAPES
	map_block Field_Map107, .LField_Map107_GRID
	map_block Field_Map107, .LField_Map107_ANIMATION
	map_block Field_Map107
	map_block Field_Map107, .LField_Map107_MARKERS
.LField_Map107_METATILES:
	.incbin "MAP/MAP_107/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map107_SHAPES:
	.incbin "MAP/MAP_107/SHAPES.table.lz"
	.balign 4, 0
.LField_Map107_GRID:
	.incbin "MAP/MAP_107/GRID.parts.lz"
	.balign 4, 0
.LField_Map107_ANIMATION:
	.incbin "MAP/MAP_107/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map107_MARKERS:
	.incbin "MAP/MAP_107/MARKERS.table"
