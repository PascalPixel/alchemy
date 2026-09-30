@ Field_Map019: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map019
Field_Map019:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map019, .LField_Map019_METATILES
	map_block Field_Map019, .LField_Map019_SHAPES
	map_block Field_Map019, .LField_Map019_GRID
	map_block Field_Map019, .LField_Map019_ANIMATION
	map_block Field_Map019, .LField_Map019_BLEND
	map_block Field_Map019, .LField_Map019_MARKERS
.LField_Map019_METATILES:
	.incbin "MAP/MAP_019/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map019_SHAPES:
	.incbin "COMMON/MAP/MAP_001/SHAPES.table.lz"
	.balign 4, 0
.LField_Map019_GRID:
	.incbin "MAP/MAP_019/GRID.parts.lz"
	.balign 4, 0
.LField_Map019_ANIMATION:
	.incbin "MAP/MAP_019/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map019_BLEND:
	.incbin "MAP/MAP_010/BLEND.script.lz"
	.balign 4, 0
.LField_Map019_MARKERS:
	.incbin "MAP/MAP_019/MARKERS.table"
