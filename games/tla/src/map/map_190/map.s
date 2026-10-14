	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map190
Field_Map190:
	map_header 0, 0, 76, 128, 3, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map190, .LField_Map190_METATILES
	map_block Field_Map190, .LField_Map190_SHAPES
	map_block Field_Map190, .LField_Map190_GRID
	map_block Field_Map190, .LField_Map190_SHAPE_GRID
	map_block Field_Map190, .LField_Map190_ANIMATION
	map_block Field_Map190, .LField_Map190_BLEND
	map_block Field_Map190, .LField_Map190_SCENE
.LField_Map190_METATILES:
	.incbin "games/tla/src/map/map_190/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map190_SHAPES:
	.incbin "games/tla/src/map/map_190/shapes.table.lz"
	.balign 4, 0
.LField_Map190_GRID:
	.incbin "games/tla/src/map/map_190/grid.parts.lz"
	.balign 4, 0
.LField_Map190_SHAPE_GRID:
	.incbin "games/tla/src/map/map_190/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map190_ANIMATION:
	.incbin "games/tla/src/map/map_190/animation.script.lz"
	.balign 4, 0
.LField_Map190_BLEND:
	.incbin "games/tla/src/map/map_190/blend.script.lz"
	.balign 4, 0
.LField_Map190_SCENE:
	.incbin "games/tla/src/map/map_190/scene.parts.lz"
