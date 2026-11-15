	.global Field_AirsRockInside7
Field_AirsRockInside7:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInside7, .LField_AirsRockInside7_CELL_PICTURES
	map_block Field_AirsRockInside7, .LField_AirsRockInside7_SHAPES
	map_block Field_AirsRockInside7, .LField_AirsRockInside7_GRID
	map_block Field_AirsRockInside7, .LField_AirsRockInside7_SHAPE_GRID
	map_block Field_AirsRockInside7, .LField_AirsRockInside7_ANIMATION
	map_block Field_AirsRockInside7, .LField_AirsRockInside7_BLEND
	map_block Field_AirsRockInside7, .LField_AirsRockInside7_SCENE
.LField_AirsRockInside7_CELL_PICTURES:
	.incbin "games/tla/src/map/airs_rock_inside_7/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AirsRockInside7_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_7/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInside7_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_7/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInside7_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_7/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInside7_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_1/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInside7_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInside7_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_7/scene.scene.lz"
	.balign 4, 0
