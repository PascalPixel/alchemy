@ Field_Map120: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map120
Field_Map120:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map120, .LField_Map120_METATILES
	map_block Field_Map120, .LField_Map120_SHAPES
	map_block Field_Map120, .LField_Map120_GRID
	map_block Field_Map120, .LField_Map120_ANIMATION
	map_block Field_Map120, .LField_Map120_BLEND
	map_block Field_Map120, .LField_Map120_MARKERS
.LField_Map120_METATILES:
	.incbin "MAP/MAP_120/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map120_SHAPES:
	.incbin "MAP/MAP_120/SHAPES.table.lz"
	.balign 4, 0
.LField_Map120_GRID:
	.incbin "MAP/MAP_120/GRID.parts.lz"
	.balign 4, 0
.LField_Map120_ANIMATION:
	.incbin "MAP/MAP_094/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map120_BLEND:
	.incbin "COMMON/MAP/MAP_018/BLEND.script.lz"
	.balign 4, 0
.LField_Map120_MARKERS:
	.incbin "MAP/MAP_120/MARKERS.table"
