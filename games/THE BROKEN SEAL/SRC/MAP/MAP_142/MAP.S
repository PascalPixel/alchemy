@ Field_Map142: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map142
Field_Map142:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map142, .LField_Map142_METATILES
	map_block Field_Map142, .LField_Map142_SHAPES
	map_block Field_Map142, .LField_Map142_GRID
	map_block Field_Map142, .LField_Map142_ANIMATION
	map_block Field_Map142, .LField_Map142_BLEND
	map_block Field_Map142, .LField_Map142_MARKERS
.LField_Map142_METATILES:
	.incbin "MAP/MAP_142/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map142_SHAPES:
	.incbin "COMMON/MAP/MAP_062/SHAPES.table.lz"
	.balign 4, 0
.LField_Map142_GRID:
	.incbin "MAP/MAP_142/GRID.parts.lz"
	.balign 4, 0
.LField_Map142_ANIMATION:
	.incbin "COMMON/MAP/MAP_029/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map142_BLEND:
	.incbin "MAP/MAP_139/BLEND.script.lz"
	.balign 4, 0
.LField_Map142_MARKERS:
	.incbin "MAP/MAP_142/MARKERS.table"
