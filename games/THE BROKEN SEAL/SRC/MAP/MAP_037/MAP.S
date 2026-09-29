@ Field_Map037: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map037
Field_Map037:
	map_header 0, 0, 58, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map037, .LField_Map037_METATILES
	map_block Field_Map037, .LField_Map037_SHAPES
	map_block Field_Map037, .LField_Map037_GRID
	map_block Field_Map037, .LField_Map037_ANIMATION
	map_block Field_Map037, .LField_Map037_BLEND
	map_block Field_Map037, .LField_Map037_MARKERS
.LField_Map037_METATILES:
	.incbin "MAP/MAP_037/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map037_SHAPES:
	.incbin "MAP/MAP_037/SHAPES.table.lz"
	.balign 4, 0
.LField_Map037_GRID:
	.incbin "MAP/MAP_037/GRID.parts.lz"
	.balign 4, 0
.LField_Map037_ANIMATION:
	.incbin "MAP/MAP_037/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map037_BLEND:
	.incbin "MAP/MAP_037/BLEND.script.lz"
	.balign 4, 0
.LField_Map037_MARKERS:
	.incbin "MAP/MAP_037/MARKERS.table"
