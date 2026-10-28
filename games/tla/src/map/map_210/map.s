	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map210
Field_Map210:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map210, .LField_Map210_CELL_PICTURES
	map_block Field_Map210, .LField_Map210_SHAPES
	map_block Field_Map210, .LField_Map210_GRID
	map_block Field_Map210, .LField_Map210_SHAPE_GRID
	map_block Field_Map210, .LField_Map210_ANIMATION
	map_block Field_Map210, .LField_Map210_BLEND
	map_block Field_Map210, .LField_Map210_SCENE
.LField_Map210_CELL_PICTURES:
	.incbin "games/tla/src/map/map_210/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map210_SHAPES:
	.incbin "games/tla/src/map/map_210/shapes.table.lz"
	.balign 4, 0
.LField_Map210_GRID:
	.incbin "games/tla/src/map/map_210/grid.parts.lz"
	.balign 4, 0
.LField_Map210_SHAPE_GRID:
	.incbin "games/tla/src/map/map_210/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map210_ANIMATION:
	.incbin "games/tla/src/map/map_210/animation.script.lz"
	.balign 4, 0
.LField_Map210_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map210_SCENE:
	.incbin "games/tla/src/map/map_210/scene.parts.lz"
