@ Field_Map139: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map139
Field_Map139:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map139, .LField_Map139_METATILES
	map_block Field_Map139, .LField_Map139_SHAPES
	map_block Field_Map139, .LField_Map139_GRID
	map_block Field_Map139, .LField_Map139_ANIMATION
	map_block Field_Map139, .LField_Map139_BLEND
	map_block Field_Map139, .LField_Map139_MARKERS
.LField_Map139_METATILES:
	.incbin "MAP/MAP_139/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map139_SHAPES:
	.incbin "MAP/MAP_139/SHAPES.table.lz"
	.balign 4, 0
.LField_Map139_GRID:
	.incbin "MAP/MAP_139/GRID.parts.lz"
	.balign 4, 0
.LField_Map139_ANIMATION:
	.incbin "MAP/MAP_029/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map139_BLEND:
	.incbin "MAP/MAP_139/BLEND.script.lz"
	.balign 4, 0
.LField_Map139_MARKERS:
	.incbin "MAP/MAP_139/MARKERS.table"
