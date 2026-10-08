	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map160
Field_Map160:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map160, .LField_Map160_METATILES
	map_block Field_Map160, .LField_Map160_SHAPES
	map_block Field_Map160, .LField_Map160_GRID
	map_block Field_Map160, .LField_Map160_ANIMATION
	map_block Field_Map160, .LField_Map160_BLEND
	map_block Field_Map160, .LField_Map160_MARKERS
.LField_Map160_METATILES:
	.incbin "map/map_160/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map160_SHAPES:
	.incbin "map/map_160/shapes.table.lz"
	.balign 4, 0
.LField_Map160_GRID:
	.incbin "map/map_160/grid.parts.lz"
	.balign 4, 0
.LField_Map160_ANIMATION:
	.incbin "map/map_151/animation.script.lz"
	.balign 4, 0
.LField_Map160_BLEND:
	.incbin "common/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map160_MARKERS:
	.incbin "map/map_160/markers.table"
	.balign 4, 0
