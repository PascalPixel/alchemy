@ Field_Map051: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map051
Field_Map051:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map051, .LField_Map051_METATILES
	map_block Field_Map051, .LField_Map051_SHAPES
	map_block Field_Map051, .LField_Map051_GRID
	map_block Field_Map051, .LField_Map051_ANIMATION
	map_block Field_Map051, .LField_Map051_BLEND
	map_block Field_Map051, .LField_Map051_MARKERS
.LField_Map051_METATILES:
	.incbin "MAP/MAP_051/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map051_SHAPES:
	.incbin "MAP/MAP_051/SHAPES.table.lz"
	.balign 4, 0
.LField_Map051_GRID:
	.incbin "MAP/MAP_051/GRID.parts.lz"
	.balign 4, 0
.LField_Map051_ANIMATION:
	.incbin "MAP/MAP_037/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map051_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map051_MARKERS:
	.incbin "MAP/MAP_051/MARKERS.table"
