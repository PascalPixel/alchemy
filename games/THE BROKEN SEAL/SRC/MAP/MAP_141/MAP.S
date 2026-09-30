@ Field_Map141: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map141
Field_Map141:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map141, .LField_Map141_METATILES
	map_block Field_Map141, .LField_Map141_SHAPES
	map_block Field_Map141, .LField_Map141_GRID
	map_block Field_Map141, .LField_Map141_ANIMATION
	map_block Field_Map141, .LField_Map141_BLEND
	map_block Field_Map141, .LField_Map141_MARKERS
.LField_Map141_METATILES:
	.incbin "MAP/MAP_141/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map141_SHAPES:
	.incbin "MAP/MAP_141/SHAPES.table.lz"
	.balign 4, 0
.LField_Map141_GRID:
	.incbin "MAP/MAP_141/GRID.parts.lz"
	.balign 4, 0
.LField_Map141_ANIMATION:
	.incbin "COMMON/MAP/MAP_029/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map141_BLEND:
	.incbin "MAP/MAP_139/BLEND.script.lz"
	.balign 4, 0
.LField_Map141_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
