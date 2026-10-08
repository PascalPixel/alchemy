	.include "games/tbs/src/map/map.inc"
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
	.incbin "map/map_063/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map063_SHAPES:
	.incbin "map/map_063/shapes.table.lz"
	.balign 4, 0
.LField_Map063_GRID:
	.incbin "map/map_063/grid.parts.lz"
	.balign 4, 0
.LField_Map063_ANIMATION:
	.incbin "map/map_033/animation.script.lz"
	.balign 4, 0
.LField_Map063_BLEND:
	.incbin "map/map_063/blend.script.lz"
	.balign 4, 0
.LField_Map063_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
