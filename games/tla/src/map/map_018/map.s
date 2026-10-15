	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map018
Field_Map018:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map018, .LField_Map018_METATILES
	map_block Field_Map018, .LField_Map018_SHAPES
	map_block Field_Map018, .LField_Map018_GRID
	map_block Field_Map018, .LField_Map018_SHAPE_GRID
	map_block Field_Map018, .LField_Map018_ANIMATION
	map_block Field_Map018, .LField_Map018_BLEND
	map_block Field_Map018, .LField_Map018_SCENE
.LField_Map018_METATILES:
	.incbin "games/tla/src/map/map_018/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map018_SHAPES:
	.incbin "games/tla/src/map/map_018/shapes.table.lz"
	.balign 4, 0
.LField_Map018_GRID:
	.incbin "games/tla/src/map/map_018/grid.parts.lz"
	.balign 4, 0
.LField_Map018_SHAPE_GRID:
	.incbin "games/tla/src/map/map_018/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map018_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map018_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map018_SCENE:
	.incbin "games/tla/src/map/map_018/scene.parts.lz"
	.balign 4, 0
