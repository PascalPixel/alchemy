	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map053
Field_Map053:
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 70, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map053, .LField_Map053_METATILES
	map_block Field_Map053, .LField_Map053_SHAPES
	map_block Field_Map053, .LField_Map053_GRID
	map_block Field_Map053, .LField_Map053_ANIMATION
	map_block Field_Map053, .LField_Map053_BLEND
	map_block Field_Map053, .LField_Map053_MARKERS
.LField_Map053_METATILES:
	.incbin "map/map_053/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map053_SHAPES:
	.incbin "common/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map053_GRID:
	.incbin "map/map_053/grid.parts.lz"
	.balign 4, 0
.LField_Map053_ANIMATION:
	.incbin "map/map_053/animation.script.lz"
	.balign 4, 0
.LField_Map053_BLEND:
	.incbin "map/map_053/blend.script.lz"
	.balign 4, 0
.LField_Map053_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
