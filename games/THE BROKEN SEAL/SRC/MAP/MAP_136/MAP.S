@ Field_Map136: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map136
Field_Map136:
	map_header 0, 0, 96, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map136, .LField_Map136_METATILES
	map_block Field_Map136, .LField_Map136_SHAPES
	map_block Field_Map136, .LField_Map136_GRID
	map_block Field_Map136, .LField_Map136_ANIMATION
	map_block Field_Map136, .LField_Map136_BLEND
	map_block Field_Map136, .LField_Map136_MARKERS
.LField_Map136_METATILES:
	.incbin "MAP/MAP_136/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map136_SHAPES:
	.incbin "MAP/MAP_136/SHAPES.table.lz"
	.balign 4, 0
.LField_Map136_GRID:
	.incbin "MAP/MAP_136/GRID.parts.lz"
	.balign 4, 0
.LField_Map136_ANIMATION:
	.incbin "MAP/MAP_136/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map136_BLEND:
	.incbin "MAP/MAP_136/BLEND.script.lz"
	.balign 4, 0
.LField_Map136_MARKERS:
	.incbin "MAP/MAP_136/MARKERS.table"
