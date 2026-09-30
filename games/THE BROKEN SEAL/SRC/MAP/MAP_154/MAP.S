@ Field_Map154: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map154
Field_Map154:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map154, .LField_Map154_METATILES
	map_block Field_Map154, .LField_Map154_SHAPES
	map_block Field_Map154, .LField_Map154_GRID
	map_block Field_Map154, .LField_Map154_ANIMATION
	map_block Field_Map154, .LField_Map154_BLEND
	map_block Field_Map154, .LField_Map154_MARKERS
.LField_Map154_METATILES:
	.incbin "MAP/MAP_154/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map154_SHAPES:
	.incbin "COMMON/MAP/MAP_154/SHAPES.table.lz"
	.balign 4, 0
.LField_Map154_GRID:
	.incbin "MAP/MAP_154/GRID.parts.lz"
	.balign 4, 0
.LField_Map154_ANIMATION:
	.incbin "COMMON/MAP/MAP_154/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map154_BLEND:
	.incbin "COMMON/MAP/MAP_154/BLEND.script.lz"
	.balign 4, 0
.LField_Map154_MARKERS:
	.incbin "MAP/MAP_154/MARKERS.table"
