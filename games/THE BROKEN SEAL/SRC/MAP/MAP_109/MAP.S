@ Field_Map109: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map109
Field_Map109:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map109, .LField_Map109_METATILES
	map_block Field_Map109, .LField_Map109_SHAPES
	map_block Field_Map109, .LField_Map109_GRID
	map_block Field_Map109, .LField_Map109_ANIMATION
	map_block Field_Map109, .LField_Map109_BLEND
	map_block Field_Map109, .LField_Map109_MARKERS
.LField_Map109_METATILES:
	.incbin "MAP/MAP_109/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map109_SHAPES:
	.incbin "MAP/MAP_109/SHAPES.table.lz"
	.balign 4, 0
.LField_Map109_GRID:
	.incbin "MAP/MAP_109/GRID.parts.lz"
	.balign 4, 0
.LField_Map109_ANIMATION:
	.incbin "MAP/MAP_109/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map109_BLEND:
	.incbin "MAP/MAP_109/BLEND.script.lz"
	.balign 4, 0
.LField_Map109_MARKERS:
	.incbin "MAP/MAP_109/MARKERS.table"
