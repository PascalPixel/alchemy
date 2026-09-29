@ Field_Map094: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map094
Field_Map094:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map094, .LField_Map094_METATILES
	map_block Field_Map094, .LField_Map094_SHAPES
	map_block Field_Map094, .LField_Map094_GRID
	map_block Field_Map094, .LField_Map094_ANIMATION
	map_block Field_Map094, .LField_Map094_BLEND
	map_block Field_Map094, .LField_Map094_MARKERS
.LField_Map094_METATILES:
	.incbin "MAP/MAP_094/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map094_SHAPES:
	.incbin "MAP/MAP_094/SHAPES.table.lz"
	.balign 4, 0
.LField_Map094_GRID:
	.incbin "MAP/MAP_094/GRID.parts.lz"
	.balign 4, 0
.LField_Map094_ANIMATION:
	.incbin "MAP/MAP_094/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map094_BLEND:
	.incbin "MAP/MAP_094/BLEND.script.lz"
	.balign 4, 0
.LField_Map094_MARKERS:
	.incbin "MAP/MAP_094/MARKERS.table"
