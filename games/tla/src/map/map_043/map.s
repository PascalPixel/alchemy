	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map043
Field_Map043:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map043, .LField_Map043_CELL_PICTURES
	map_block Field_Map043, .LField_Map043_SHAPES
	map_block Field_Map043, .LField_Map043_GRID
	map_block Field_Map043, .LField_Map043_SHAPE_GRID
	map_block Field_Map043, .LField_Map043_ANIMATION
	map_block Field_Map043, .LField_Map043_BLEND
	map_block Field_Map043, .LField_Map043_SCENE
.LField_Map043_CELL_PICTURES:
	.incbin "games/tla/src/map/map_043/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map043_SHAPES:
	.incbin "games/tla/src/map/map_043/shapes.table.lz"
	.balign 4, 0
.LField_Map043_GRID:
	.incbin "games/tla/src/map/map_043/grid.parts.lz"
	.balign 4, 0
.LField_Map043_SHAPE_GRID:
	.incbin "games/tla/src/map/map_043/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map043_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map043_BLEND:
	.incbin "games/tla/src/map/map_043/blend.script.lz"
	.balign 4, 0
.LField_Map043_SCENE:
	.incbin "games/tla/src/map/map_043/scene.parts.lz"
