	.global Field_AirsRockInside2
Field_AirsRockInside2:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInside2, .LField_AirsRockInside2_CELL_PICTURES
	map_block Field_AirsRockInside2, .LField_AirsRockInside2_SHAPES
	map_block Field_AirsRockInside2, .LField_AirsRockInside2_GRID
	map_block Field_AirsRockInside2, .LField_AirsRockInside2_SHAPE_GRID
	map_block Field_AirsRockInside2, .LField_AirsRockInside2_ANIMATION
	map_block Field_AirsRockInside2, .LField_AirsRockInside2_BLEND
	map_block Field_AirsRockInside2, .LField_AirsRockInside2_SCENE
.LField_AirsRockInside2_CELL_PICTURES:
	.incbin "games/tla/src/map/airs_rock_inside_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AirsRockInside2_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_2/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInside2_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_2/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInside2_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInside2_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_1/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInside2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInside2_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_2/scene.scene.lz"
