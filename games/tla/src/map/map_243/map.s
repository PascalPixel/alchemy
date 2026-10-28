	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map243
Field_Map243:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map243, .LField_Map243_CELL_PICTURES
	map_block Field_Map243, .LField_Map243_SHAPES
	map_block Field_Map243, .LField_Map243_GRID
	map_block Field_Map243, .LField_Map243_SHAPE_GRID
	map_block Field_Map243, .LField_Map243_ANIMATION
	map_block Field_Map243
	map_block Field_Map243, .LField_Map243_SCENE
.LField_Map243_CELL_PICTURES:
	.incbin "games/tla/src/map/map_243/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map243_SHAPES:
	.incbin "games/tla/src/map/map_243/shapes.table.lz"
	.balign 4, 0
.LField_Map243_GRID:
	.incbin "games/tla/src/map/map_243/grid.parts.lz"
	.balign 4, 0
.LField_Map243_SHAPE_GRID:
	.incbin "games/tla/src/map/map_243/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map243_ANIMATION:
	.incbin "games/tla/src/map/map_243/animation.script.lz"
	.balign 4, 0
.LField_Map243_SCENE:
	.incbin "games/tla/src/map/map_243/scene.parts.lz"
	.balign 4, 0
