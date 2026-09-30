@ Field_Map057: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map057
Field_Map057:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map057, .LField_Map057_METATILES
	map_block Field_Map057, .LField_Map057_SHAPES
	map_block Field_Map057, .LField_Map057_GRID
	map_block Field_Map057, .LField_Map057_ANIMATION
	map_block Field_Map057, .LField_Map057_BLEND
	map_block Field_Map057, .LField_Map057_MARKERS
.LField_Map057_METATILES:
	.incbin "MAP/MAP_057/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map057_SHAPES:
	.incbin "MAP/MAP_057/SHAPES.table.lz"
	.balign 4, 0
.LField_Map057_GRID:
	.incbin "MAP/MAP_057/GRID.parts.lz"
	.balign 4, 0
.LField_Map057_ANIMATION:
	.incbin "MAP/MAP_054/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map057_BLEND:
	.incbin "COMMON/MAP/MAP_054/BLEND.script.lz"
	.balign 4, 0
.LField_Map057_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
