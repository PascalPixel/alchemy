@ Field_Map105: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map105
Field_Map105:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map105, .LField_Map105_METATILES
	map_block Field_Map105, .LField_Map105_SHAPES
	map_block Field_Map105, .LField_Map105_GRID
	map_block Field_Map105, .LField_Map105_ANIMATION
	map_block Field_Map105, .LField_Map105_BLEND
	map_block Field_Map105, .LField_Map105_MARKERS
.LField_Map105_METATILES:
	.incbin "MAP/MAP_105/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map105_SHAPES:
	.incbin "MAP/MAP_105/SHAPES.table.lz"
	.balign 4, 0
.LField_Map105_GRID:
	.incbin "MAP/MAP_105/GRID.parts.lz"
	.balign 4, 0
.LField_Map105_ANIMATION:
	.incbin "MAP/MAP_105/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map105_BLEND:
	.incbin "MAP/MAP_105/BLEND.script.lz"
	.balign 4, 0
.LField_Map105_MARKERS:
	.incbin "MAP/MAP_105/MARKERS.table"
