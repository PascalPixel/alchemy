@ Field_Map055: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map055
Field_Map055:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map055, .LField_Map055_METATILES
	map_block Field_Map055, .LField_Map055_SHAPES
	map_block Field_Map055, .LField_Map055_GRID
	map_block Field_Map055, .LField_Map055_ANIMATION
	map_block Field_Map055, .LField_Map055_BLEND
	map_block Field_Map055, .LField_Map055_MARKERS
.LField_Map055_METATILES:
	.incbin "MAP/MAP_055/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map055_SHAPES:
	.incbin "MAP/MAP_055/SHAPES.table.lz"
	.balign 4, 0
.LField_Map055_GRID:
	.incbin "MAP/MAP_055/GRID.parts.lz"
	.balign 4, 0
.LField_Map055_ANIMATION:
	.incbin "MAP/MAP_054/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map055_BLEND:
	.incbin "MAP/MAP_054/BLEND.script.lz"
	.balign 4, 0
.LField_Map055_MARKERS:
	.incbin "MAP/MAP_055/MARKERS.table"
