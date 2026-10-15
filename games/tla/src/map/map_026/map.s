	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map026
Field_Map026:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map026, .LField_Map026_METATILES
	map_block Field_Map026, .LField_Map026_SHAPES
	map_block Field_Map026, .LField_Map026_GRID
	map_block Field_Map026, .LField_Map026_SHAPE_GRID
	map_block Field_Map026, .LField_Map026_ANIMATION
	map_block Field_Map026, .LField_Map026_BLEND
	map_block Field_Map026, .LField_Map026_SCENE
.LField_Map026_METATILES:
	.incbin "games/tla/src/map/map_026/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map026_SHAPES:
	.incbin "games/tla/src/map/map_026/shapes.table.lz"
	.balign 4, 0
.LField_Map026_GRID:
	.incbin "games/tla/src/map/map_026/grid.parts.lz"
	.balign 4, 0
.LField_Map026_SHAPE_GRID:
	.incbin "games/tla/src/map/map_026/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map026_ANIMATION:
	.incbin "games/tla/src/map/map_024/animation.script.lz"
	.balign 4, 0
.LField_Map026_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map026_SCENE:
	.incbin "games/tla/src/map/map_026/scene.parts.lz"
	.balign 4, 0
