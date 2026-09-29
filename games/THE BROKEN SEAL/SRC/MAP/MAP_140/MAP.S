@ Field_Map140: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map140
Field_Map140:
	.ifdef TBS_EDITION_JA
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map140, .LField_Map140_METATILES
	map_block Field_Map140, .LField_Map140_SHAPES
	map_block Field_Map140, .LField_Map140_GRID
	map_block Field_Map140, .LField_Map140_ANIMATION
	map_block Field_Map140, .LField_Map140_BLEND
	map_block Field_Map140, .LField_Map140_MARKERS
.LField_Map140_METATILES:
	.incbin "MAP/MAP_140/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map140_SHAPES:
	.incbin "MAP/MAP_062/SHAPES.table.lz"
	.balign 4, 0
.LField_Map140_GRID:
	.incbin "MAP/MAP_140/GRID_JA.parts.lz"
	.balign 4, 0
.LField_Map140_ANIMATION:
	.incbin "MAP/MAP_029/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map140_BLEND:
	.incbin "MAP/MAP_139/BLEND.script.lz"
	.balign 4, 0
.LField_Map140_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
	.else
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map140, .LField_Map140_METATILES
	map_block Field_Map140, .LField_Map140_SHAPES
	map_block Field_Map140, .LField_Map140_GRID
	map_block Field_Map140, .LField_Map140_ANIMATION
	map_block Field_Map140, .LField_Map140_BLEND
	map_block Field_Map140, .LField_Map140_MARKERS
.LField_Map140_METATILES:
	.incbin "MAP/MAP_140/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map140_SHAPES:
	.incbin "MAP/MAP_062/SHAPES.table.lz"
	.balign 4, 0
.LField_Map140_GRID:
	.incbin "MAP/MAP_140/GRID.parts.lz"
	.balign 4, 0
.LField_Map140_ANIMATION:
	.incbin "MAP/MAP_029/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map140_BLEND:
	.incbin "MAP/MAP_139/BLEND.script.lz"
	.balign 4, 0
.LField_Map140_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
	.endif
