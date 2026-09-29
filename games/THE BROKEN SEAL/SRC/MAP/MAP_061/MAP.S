@ Field_Map061: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map061
Field_Map061:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map061, .LField_Map061_METATILES
	map_block Field_Map061, .LField_Map061_SHAPES
	map_block Field_Map061, .LField_Map061_GRID
	map_block Field_Map061, .LField_Map061_ANIMATION
	map_block Field_Map061, .LField_Map061_BLEND
	map_block Field_Map061, .LField_Map061_MARKERS
.LField_Map061_METATILES:
	.incbin "MAP/MAP_061/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map061_SHAPES:
	.incbin "MAP/MAP_061/SHAPES.table.lz"
	.balign 4, 0
.LField_Map061_GRID:
	.incbin "MAP/MAP_061/GRID.parts.lz"
	.balign 4, 0
.LField_Map061_ANIMATION:
	.incbin "MAP/MAP_061/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map061_BLEND:
	.incbin "MAP/MAP_061/BLEND.script.lz"
	.balign 4, 0
.LField_Map061_MARKERS:
	.incbin "MAP/MAP_061/MARKERS.table"
