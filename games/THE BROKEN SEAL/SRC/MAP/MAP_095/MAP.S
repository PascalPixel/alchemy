@ Field_Map095: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map095
Field_Map095:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map095, .LField_Map095_METATILES
	map_block Field_Map095, .LField_Map095_SHAPES
	map_block Field_Map095, .LField_Map095_GRID
	map_block Field_Map095, .LField_Map095_ANIMATION
	map_block Field_Map095, .LField_Map095_BLEND
	map_block Field_Map095, .LField_Map095_MARKERS
.LField_Map095_METATILES:
	.incbin "MAP/MAP_095/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map095_SHAPES:
	.incbin "MAP/MAP_095/SHAPES.table.lz"
	.balign 4, 0
.LField_Map095_GRID:
	.incbin "MAP/MAP_095/GRID.parts.lz"
	.balign 4, 0
.LField_Map095_ANIMATION:
	.incbin "MAP/MAP_095/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map095_BLEND:
	.incbin "MAP/MAP_095/BLEND.script.lz"
	.balign 4, 0
.LField_Map095_MARKERS:
	.incbin "MAP/MAP_095/MARKERS.table"
