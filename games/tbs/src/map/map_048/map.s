	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map048
Field_Map048:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map048, .LField_Map048_METATILES
	map_block Field_Map048, .LField_Map048_SHAPES
	map_block Field_Map048, .LField_Map048_GRID
	map_block Field_Map048, .LField_Map048_ANIMATION
	map_block Field_Map048, .LField_Map048_BLEND
	map_block Field_Map048, .LField_Map048_MARKERS
.LField_Map048_METATILES:
	.incbin "map/map_048/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map048_SHAPES:
	.incbin "map/map_048/shapes.table.lz"
	.balign 4, 0
.LField_Map048_GRID:
	.incbin "map/map_048/grid.parts.lz"
	.balign 4, 0
.LField_Map048_ANIMATION:
	.incbin "map/map_048/animation.script.lz"
	.balign 4, 0
.LField_Map048_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map048_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
