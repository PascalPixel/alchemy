	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map228
Field_Map228:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map228, .LField_Map228_CELL_PICTURES
	map_block Field_Map228, .LField_Map228_SHAPES
	map_block Field_Map228, .LField_Map228_GRID
	map_block Field_Map228, .LField_Map228_SHAPE_GRID
	map_block Field_Map228, .LField_Map228_ANIMATION
	map_block Field_Map228
	map_block Field_Map228, .LField_Map228_SCENE
.LField_Map228_CELL_PICTURES:
	.incbin "games/tla/src/map/map_228/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map228_SHAPES:
	.incbin "games/tla/src/map/map_228/shapes.table.lz"
	.balign 4, 0
.LField_Map228_GRID:
	.incbin "games/tla/src/map/map_228/grid.parts.lz"
	.balign 4, 0
.LField_Map228_SHAPE_GRID:
	.incbin "games/tla/src/map/map_228/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map228_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map228_SCENE:
	.incbin "games/tla/src/map/map_228/scene.parts.lz"
	.balign 4, 0
