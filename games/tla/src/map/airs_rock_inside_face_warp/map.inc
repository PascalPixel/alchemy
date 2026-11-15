	.global Field_AirsRockInsideFaceWarp
Field_AirsRockInsideFaceWarp:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInsideFaceWarp, .LField_AirsRockInsideFaceWarp_CELL_PICTURES
	map_block Field_AirsRockInsideFaceWarp, .LField_AirsRockInsideFaceWarp_SHAPES
	map_block Field_AirsRockInsideFaceWarp, .LField_AirsRockInsideFaceWarp_GRID
	map_block Field_AirsRockInsideFaceWarp, .LField_AirsRockInsideFaceWarp_SHAPE_GRID
	map_block Field_AirsRockInsideFaceWarp, .LField_AirsRockInsideFaceWarp_ANIMATION
	map_block Field_AirsRockInsideFaceWarp, .LField_AirsRockInsideFaceWarp_BLEND
	map_block Field_AirsRockInsideFaceWarp, .LField_AirsRockInsideFaceWarp_SCENE
.LField_AirsRockInsideFaceWarp_CELL_PICTURES:
	.incbin "games/tla/src/map/airs_rock_inside_face_warp/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AirsRockInsideFaceWarp_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_face_warp/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInsideFaceWarp_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_face_warp/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInsideFaceWarp_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_face_warp/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInsideFaceWarp_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_hall/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInsideFaceWarp_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInsideFaceWarp_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_face_warp/scene.scene.lz"
	.balign 4, 0
