	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map231
Field_Map231:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map231, .LField_Map231_CELL_PICTURES
	map_block Field_Map231, .LField_Map231_SHAPES
	map_block Field_Map231, .LField_Map231_GRID
	map_block Field_Map231, .LField_Map231_SHAPE_GRID
	map_block Field_Map231, .LField_Map231_ANIMATION
	map_block Field_Map231
	map_block Field_Map231, .LField_Map231_SCENE
.LField_Map231_CELL_PICTURES:
	.incbin "games/tla/src/map/map_231/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map231_SHAPES:
	.incbin "games/tla/src/map/map_231/shapes.table.lz"
	.balign 4, 0
.LField_Map231_GRID:
	.incbin "games/tla/src/map/map_231/grid.parts.lz"
	.balign 4, 0
.LField_Map231_SHAPE_GRID:
	.incbin "games/tla/src/map/map_231/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map231_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map231_SCENE:
	.incbin "games/tla/src/map/map_231/scene.parts.lz"
	.balign 4, 0
