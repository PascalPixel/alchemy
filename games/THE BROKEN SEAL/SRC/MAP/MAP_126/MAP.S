@ Field_Map126: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map126
Field_Map126:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map126, .LField_Map126_METATILES
	map_block Field_Map126, .LField_Map126_SHAPES
	map_block Field_Map126, .LField_Map126_GRID
	map_block Field_Map126, .LField_Map126_ANIMATION
	map_block Field_Map126, .LField_Map126_BLEND
	map_block Field_Map126, .LField_Map126_MARKERS
.LField_Map126_METATILES:
	.incbin "MAP/MAP_126/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map126_SHAPES:
	.incbin "MAP/MAP_126/SHAPES.table.lz"
	.balign 4, 0
.LField_Map126_GRID:
	.incbin "MAP/MAP_126/GRID.parts.lz"
	.balign 4, 0
.LField_Map126_ANIMATION:
	.incbin "MAP/MAP_093/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map126_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map126_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
