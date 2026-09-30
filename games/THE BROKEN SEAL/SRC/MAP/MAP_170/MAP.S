@ Field_Map170: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map170
Field_Map170:
	map_header 0, 0, 30, 22, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 132, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map170, .LField_Map170_METATILES
	map_block Field_Map170, .LField_Map170_SHAPES
	map_block Field_Map170, .LField_Map170_GRID
	map_block Field_Map170, .LField_Map170_ANIMATION
	map_block Field_Map170, .LField_Map170_BLEND
	map_block Field_Map170, .LField_Map170_MARKERS
.LField_Map170_METATILES:
	.incbin "MAP/MAP_170/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map170_SHAPES:
	.incbin "COMMON/MAP/MAP_170/SHAPES.table.lz"
	.balign 4, 0
.LField_Map170_GRID:
	.incbin "MAP/MAP_170/GRID.parts.lz"
	.balign 4, 0
.LField_Map170_ANIMATION:
	.incbin "COMMON/MAP/MAP_170/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map170_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map170_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
