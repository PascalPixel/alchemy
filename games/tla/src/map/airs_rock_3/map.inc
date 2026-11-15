	.global Field_AirsRock3
Field_AirsRock3:
	map_header 0, 0, 128, 128, 2, 1, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRock3, .LField_AirsRock3_CELL_PICTURES
	map_block Field_AirsRock3, .LField_AirsRock3_SHAPES
	map_block Field_AirsRock3, .LField_AirsRock3_GRID
	map_block Field_AirsRock3, .LField_AirsRock3_SHAPE_GRID
	map_block Field_AirsRock3, .LField_AirsRock3_ANIMATION
	map_block Field_AirsRock3, .LField_AirsRock3_BLEND
	map_block Field_AirsRock3, .LField_AirsRock3_SCENE
.LField_AirsRock3_CELL_PICTURES:
	.incbin "games/tla/src/map/airs_rock_3/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AirsRock3_SHAPES:
	.incbin "games/tla/src/map/airs_rock_3/shapes.table.lz"
	.balign 4, 0
.LField_AirsRock3_GRID:
	.incbin "games/tla/src/map/airs_rock_3/map.grid.lz"
	.balign 4, 0
.LField_AirsRock3_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRock3_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_AirsRock3_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRock3_SCENE:
	.incbin "games/tla/src/map/airs_rock_3/scene.scene.lz"
	.balign 4, 0
