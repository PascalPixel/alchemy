@ Field_Map063: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map063
Field_Map063:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map063, .LField_Map063_METATILES
	map_block Field_Map063, .LField_Map063_SHAPES
	map_block Field_Map063, .LField_Map063_GRID
	map_block Field_Map063, .LField_Map063_ANIMATION
	map_block Field_Map063, .LField_Map063_BLEND
	map_block Field_Map063, .LField_Map063_MARKERS
.LField_Map063_METATILES:
	.incbin "MAP/MAP_063/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map063_SHAPES:
	.incbin "MAP/MAP_063/SHAPES.table.lz"
	.balign 4, 0
.LField_Map063_GRID:
	.incbin "MAP/MAP_063/GRID.parts.lz"
	.balign 4, 0
.LField_Map063_ANIMATION:
	.incbin "MAP/MAP_033/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map063_BLEND:
	.incbin "MAP/MAP_063/BLEND.script.lz"
	.balign 4, 0
.LField_Map063_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
