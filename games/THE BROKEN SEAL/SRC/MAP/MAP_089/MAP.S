@ Field_Map089: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map089
Field_Map089:
	map_header 0, 0, 55, 68, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map089, .LField_Map089_METATILES
	map_block Field_Map089, .LField_Map089_SHAPES
	map_block Field_Map089, .LField_Map089_GRID
	map_block Field_Map089, .LField_Map089_ANIMATION
	map_block Field_Map089, .LField_Map089_BLEND
	map_block Field_Map089, .LField_Map089_MARKERS
.LField_Map089_METATILES:
	.incbin "MAP/MAP_089/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map089_SHAPES:
	.incbin "MAP/MAP_089/SHAPES.table.lz"
	.balign 4, 0
.LField_Map089_GRID:
	.incbin "MAP/MAP_089/GRID.parts.lz"
	.balign 4, 0
.LField_Map089_ANIMATION:
	.incbin "MAP/MAP_089/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map089_BLEND:
	.incbin "MAP/MAP_089/BLEND.script.lz"
	.balign 4, 0
.LField_Map089_MARKERS:
	.incbin "MAP/MAP_089/MARKERS.table"
