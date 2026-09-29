@ Field_Map087: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map087
Field_Map087:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map087, .LField_Map087_METATILES
	map_block Field_Map087, .LField_Map087_SHAPES
	map_block Field_Map087, .LField_Map087_GRID
	map_block Field_Map087, .LField_Map087_ANIMATION
	map_block Field_Map087, .LField_Map087_BLEND
	map_block Field_Map087, .LField_Map087_MARKERS
.LField_Map087_METATILES:
	.incbin "MAP/MAP_087/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map087_SHAPES:
	.incbin "MAP/MAP_087/SHAPES.table.lz"
	.balign 4, 0
.LField_Map087_GRID:
	.incbin "MAP/MAP_087/GRID.parts.lz"
	.balign 4, 0
.LField_Map087_ANIMATION:
	.incbin "MAP/MAP_080/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map087_BLEND:
	.incbin "MAP/MAP_025/BLEND.script.lz"
	.balign 4, 0
.LField_Map087_MARKERS:
	.incbin "MAP/MAP_087/MARKERS.table"
