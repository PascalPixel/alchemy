	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map042
Field_Map042:
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map042, .LField_Map042_METATILES
	map_block Field_Map042, .LField_Map042_SHAPES
	map_block Field_Map042, .LField_Map042_GRID
	map_block Field_Map042, .LField_Map042_ANIMATION
	map_block Field_Map042, .LField_Map042_BLEND
	map_block Field_Map042, .LField_Map042_MARKERS
.LField_Map042_METATILES:
	.incbin "map/map_042/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map042_SHAPES:
	.incbin "map/map_042/shapes.table.lz"
	.balign 4, 0
.LField_Map042_GRID:
	.incbin "map/map_042/grid.parts.lz"
	.balign 4, 0
.LField_Map042_ANIMATION:
	.incbin "map/map_042/animation.script.lz"
	.balign 4, 0
.LField_Map042_BLEND:
	.incbin "map/map_039/blend.script.lz"
	.balign 4, 0
.LField_Map042_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
