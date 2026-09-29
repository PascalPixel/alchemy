@ Field_Map171: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map171
Field_Map171:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map171, .LField_Map171_METATILES
	map_block Field_Map171, .LField_Map171_SHAPES
	map_block Field_Map171, .LField_Map171_GRID
	map_block Field_Map171, .LField_Map171_ANIMATION
	map_block Field_Map171, .LField_Map171_BLEND
	map_block Field_Map171, .LField_Map171_MARKERS
.LField_Map171_METATILES:
	.incbin "MAP/MAP_171/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map171_SHAPES:
	.incbin "MAP/MAP_171/SHAPES.table.lz"
	.balign 4, 0
.LField_Map171_GRID:
	.incbin "MAP/MAP_171/GRID.parts.lz"
	.balign 4, 0
.LField_Map171_ANIMATION:
	.incbin "MAP/MAP_171/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map171_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map171_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
