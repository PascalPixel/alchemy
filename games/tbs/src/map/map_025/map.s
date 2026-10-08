	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map025
Field_Map025:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map025, .LField_Map025_METATILES
	map_block Field_Map025, .LField_Map025_SHAPES
	map_block Field_Map025, .LField_Map025_GRID
	map_block Field_Map025, .LField_Map025_ANIMATION
	map_block Field_Map025, .LField_Map025_BLEND
	map_block Field_Map025, .LField_Map025_MARKERS
.LField_Map025_METATILES:
	.incbin "map/map_025/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map025_SHAPES:
	.incbin "map/map_025/shapes.table.lz"
	.balign 4, 0
.LField_Map025_GRID:
	.incbin "map/map_025/grid.parts.lz"
	.balign 4, 0
.LField_Map025_ANIMATION:
	.incbin "map/map_025/animation.script.lz"
	.balign 4, 0
.LField_Map025_BLEND:
	.incbin "map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map025_MARKERS:
	.incbin "map/map_025/markers.table"
	.balign 4, 0
