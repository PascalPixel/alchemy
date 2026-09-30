@ Field_Map058: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map058
Field_Map058:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map058, .LField_Map058_METATILES
	map_block Field_Map058, .LField_Map058_SHAPES
	map_block Field_Map058, .LField_Map058_GRID
	map_block Field_Map058, .LField_Map058_ANIMATION
	map_block Field_Map058, .LField_Map058_BLEND
	map_block Field_Map058, .LField_Map058_MARKERS
.LField_Map058_METATILES:
	.incbin "MAP/MAP_058/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map058_SHAPES:
	.incbin "COMMON/MAP/MAP_058/SHAPES.table.lz"
	.balign 4, 0
.LField_Map058_GRID:
	.incbin "MAP/MAP_058/GRID.parts.lz"
	.balign 4, 0
.LField_Map058_ANIMATION:
	.incbin "COMMON/MAP/MAP_058/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map058_BLEND:
	.incbin "MAP/MAP_058/BLEND.script.lz"
	.balign 4, 0
.LField_Map058_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
