	.global Field_AirsRockInside6
Field_AirsRockInside6:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInside6, .LField_AirsRockInside6_CELL_PICTURES
	map_block Field_AirsRockInside6, .LField_AirsRockInside6_SHAPES
	map_block Field_AirsRockInside6, .LField_AirsRockInside6_GRID
	map_block Field_AirsRockInside6, .LField_AirsRockInside6_SHAPE_GRID
	map_block Field_AirsRockInside6, .LField_AirsRockInside6_ANIMATION
	map_block Field_AirsRockInside6, .LField_AirsRockInside6_BLEND
	map_block Field_AirsRockInside6, .LField_AirsRockInside6_SCENE
.LField_AirsRockInside6_CELL_PICTURES:
	.incbin "games/tla/src/map/airs_rock_inside_6/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AirsRockInside6_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_6/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInside6_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_6/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInside6_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_6/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInside6_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_1/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInside6_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInside6_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_6/scene.scene.lz"
