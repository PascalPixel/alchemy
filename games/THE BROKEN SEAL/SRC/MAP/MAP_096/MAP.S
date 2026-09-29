@ Field_Map096: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map096
Field_Map096:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map096, .LField_Map096_METATILES
	map_block Field_Map096, .LField_Map096_SHAPES
	map_block Field_Map096, .LField_Map096_GRID
	map_block Field_Map096, .LField_Map096_ANIMATION
	map_block Field_Map096, .LField_Map096_BLEND
	map_block Field_Map096, .LField_Map096_MARKERS
.LField_Map096_METATILES:
	.incbin "MAP/MAP_096/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map096_SHAPES:
	.incbin "MAP/MAP_096/SHAPES.table.lz"
	.balign 4, 0
.LField_Map096_GRID:
	.incbin "MAP/MAP_096/GRID.parts.lz"
	.balign 4, 0
.LField_Map096_ANIMATION:
	.incbin "MAP/MAP_093/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map096_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map096_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
