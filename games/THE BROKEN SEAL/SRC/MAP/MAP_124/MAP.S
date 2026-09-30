@ Field_Map124: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map124
Field_Map124:
	map_header 0, 0, 70, 34, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map124, .LField_Map124_METATILES
	map_block Field_Map124, .LField_Map124_SHAPES
	map_block Field_Map124, .LField_Map124_GRID
	map_block Field_Map124, .LField_Map124_ANIMATION
	map_block Field_Map124, .LField_Map124_BLEND
	map_block Field_Map124, .LField_Map124_MARKERS
.LField_Map124_METATILES:
	.incbin "MAP/MAP_124/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map124_SHAPES:
	.incbin "COMMON/MAP/MAP_001/SHAPES.table.lz"
	.balign 4, 0
.LField_Map124_GRID:
	.incbin "MAP/MAP_124/GRID.parts.lz"
	.balign 4, 0
.LField_Map124_ANIMATION:
	.incbin "MAP/MAP_124/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map124_BLEND:
	.incbin "MAP/MAP_124/BLEND.script.lz"
	.balign 4, 0
.LField_Map124_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
