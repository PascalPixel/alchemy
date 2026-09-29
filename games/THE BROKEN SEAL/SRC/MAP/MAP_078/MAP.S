@ Field_Map078: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map078
Field_Map078:
	map_header 0, 0, 64, 64, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map078, .LField_Map078_METATILES
	map_block Field_Map078, .LField_Map078_SHAPES
	map_block Field_Map078, .LField_Map078_GRID
	map_block Field_Map078, .LField_Map078_ANIMATION
	map_block Field_Map078, .LField_Map078_BLEND
	map_block Field_Map078, .LField_Map078_MARKERS
.LField_Map078_METATILES:
	.incbin "MAP/MAP_078/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map078_SHAPES:
	.incbin "MAP/MAP_078/SHAPES.table.lz"
	.balign 4, 0
.LField_Map078_GRID:
	.incbin "MAP/MAP_078/GRID.parts.lz"
	.balign 4, 0
.LField_Map078_ANIMATION:
	.incbin "MAP/MAP_078/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map078_BLEND:
	.incbin "MAP/MAP_025/BLEND.script.lz"
	.balign 4, 0
.LField_Map078_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
