@ Field_Map127: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map127
Field_Map127:
	map_header 28, 0, 180, 56, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 56, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 112, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map127, .LField_Map127_METATILES
	map_block Field_Map127, .LField_Map127_SHAPES
	map_block Field_Map127, .LField_Map127_GRID
	map_block Field_Map127, .LField_Map127_ANIMATION
	map_block Field_Map127, .LField_Map127_BLEND
	map_block Field_Map127, .LField_Map127_MARKERS
.LField_Map127_METATILES:
	.incbin "MAP/MAP_127/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map127_SHAPES:
	.incbin "MAP/MAP_127/SHAPES.table.lz"
	.balign 4, 0
.LField_Map127_GRID:
	.incbin "MAP/MAP_127/GRID.parts.lz"
	.balign 4, 0
.LField_Map127_ANIMATION:
	.incbin "MAP/MAP_127/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map127_BLEND:
	.incbin "MAP/MAP_127/BLEND.script.lz"
	.balign 4, 0
.LField_Map127_MARKERS:
	.incbin "MAP/MAP_127/MARKERS.table"
