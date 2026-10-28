	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map274
Field_Map274:
	map_header 0, 0, 204, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 160, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map274, .LField_Map274_CELL_PICTURES
	map_block Field_Map274, .LField_Map274_SHAPES
	map_block Field_Map274, .LField_Map274_GRID
	map_block Field_Map274, .LField_Map274_SHAPE_GRID
	map_block Field_Map274, .LField_Map274_ANIMATION
	map_block Field_Map274
	map_block Field_Map274, .LField_Map274_SCENE
.LField_Map274_CELL_PICTURES:
	.incbin "games/tla/src/map/map_274/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map274_SHAPES:
	.incbin "games/tla/src/map/map_274/shapes.table.lz"
	.balign 4, 0
.LField_Map274_GRID:
	.incbin "games/tla/src/map/map_274/grid.parts.lz"
	.balign 4, 0
.LField_Map274_SHAPE_GRID:
	.incbin "games/tla/src/map/map_274/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map274_ANIMATION:
	.incbin "games/tla/src/map/map_263/animation.script.lz"
	.balign 4, 0
.LField_Map274_SCENE:
	.incbin "games/tla/src/map/map_274/scene.parts.lz"
	.balign 4, 0
