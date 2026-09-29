@ Field_Map048: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map048
Field_Map048:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map048, .LField_Map048_METATILES
	map_block Field_Map048, .LField_Map048_SHAPES
	map_block Field_Map048, .LField_Map048_GRID
	map_block Field_Map048, .LField_Map048_ANIMATION
	map_block Field_Map048, .LField_Map048_BLEND
	map_block Field_Map048, .LField_Map048_MARKERS
.LField_Map048_METATILES:
	.incbin "MAP/MAP_048/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map048_SHAPES:
	.incbin "MAP/MAP_048/SHAPES.table.lz"
	.balign 4, 0
.LField_Map048_GRID:
	.incbin "MAP/MAP_048/GRID.parts.lz"
	.balign 4, 0
.LField_Map048_ANIMATION:
	.incbin "MAP/MAP_048/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map048_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map048_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
