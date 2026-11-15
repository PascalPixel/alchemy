	.global Field_AirsRockInside9
Field_AirsRockInside9:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInside9, .LField_AirsRockInside9_CELL_PICTURES
	map_block Field_AirsRockInside9, .LField_AirsRockInside9_SHAPES
	map_block Field_AirsRockInside9, .LField_AirsRockInside9_GRID
	map_block Field_AirsRockInside9, .LField_AirsRockInside9_SHAPE_GRID
	map_block Field_AirsRockInside9, .LField_AirsRockInside9_ANIMATION
	map_block Field_AirsRockInside9, .LField_AirsRockInside9_BLEND
	map_block Field_AirsRockInside9, .LField_AirsRockInside9_SCENE
.LField_AirsRockInside9_CELL_PICTURES:
	.incbin "games/tla/src/map/airs_rock_inside_9/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AirsRockInside9_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_9/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInside9_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_9/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInside9_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_9/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInside9_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_1/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInside9_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInside9_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_9/scene.scene.lz"
	.balign 4, 0
