@ Field_Map099: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map099
Field_Map099:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map099, .LField_Map099_METATILES
	map_block Field_Map099, .LField_Map099_SHAPES
	map_block Field_Map099, .LField_Map099_GRID
	map_block Field_Map099, .LField_Map099_ANIMATION
	map_block Field_Map099, .LField_Map099_BLEND
	map_block Field_Map099, .LField_Map099_MARKERS
.LField_Map099_METATILES:
	.incbin "MAP/MAP_099/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map099_SHAPES:
	.incbin "MAP/MAP_099/SHAPES.table.lz"
	.balign 4, 0
.LField_Map099_GRID:
	.incbin "MAP/MAP_099/GRID.parts.lz"
	.balign 4, 0
.LField_Map099_ANIMATION:
	.incbin "MAP/MAP_052/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map099_BLEND:
	.incbin "MAP/MAP_018/BLEND.script.lz"
	.balign 4, 0
.LField_Map099_MARKERS:
	.incbin "MAP/MAP_099/MARKERS.table"
