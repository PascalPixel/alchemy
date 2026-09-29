@ Field_Map020: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map020
Field_Map020:
	map_header 0, 0, 62, 58, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 62, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 124, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map020, .LField_Map020_METATILES
	map_block Field_Map020, .LField_Map020_SHAPES
	map_block Field_Map020, .LField_Map020_GRID
	map_block Field_Map020, .LField_Map020_ANIMATION
	map_block Field_Map020, .LField_Map020_BLEND
	map_block Field_Map020, .LField_Map020_MARKERS
.LField_Map020_METATILES:
	.incbin "MAP/MAP_020/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map020_SHAPES:
	.incbin "MAP/MAP_020/SHAPES.table.lz"
	.balign 4, 0
.LField_Map020_GRID:
	.incbin "MAP/MAP_020/GRID.parts.lz"
	.balign 4, 0
.LField_Map020_ANIMATION:
	.incbin "MAP/MAP_020/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map020_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map020_MARKERS:
	.incbin "MAP/MAP_020/MARKERS.table"
