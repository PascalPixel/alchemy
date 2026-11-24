	.global Field_AirsRockInsideFaceVents
Field_AirsRockInsideFaceVents:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInsideFaceVents, .LField_AirsRockInsideFaceVents_CELL_TILES
	map_block Field_AirsRockInsideFaceVents, .LField_AirsRockInsideFaceVents_SHAPES
	map_block Field_AirsRockInsideFaceVents, .LField_AirsRockInsideFaceVents_GRID
	map_block Field_AirsRockInsideFaceVents, .LField_AirsRockInsideFaceVents_SHAPE_GRID
	map_block Field_AirsRockInsideFaceVents, .LField_AirsRockInsideFaceVents_ANIMATION
	map_block Field_AirsRockInsideFaceVents, .LField_AirsRockInsideFaceVents_BLEND
	map_block Field_AirsRockInsideFaceVents, .LField_AirsRockInsideFaceVents_SCENE
.LField_AirsRockInsideFaceVents_CELL_TILES:
	.incbin "games/tla/src/map/airs_rock_inside_face_vents/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AirsRockInsideFaceVents_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_face_vents/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInsideFaceVents_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_face_vents/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInsideFaceVents_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_face_vents/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInsideFaceVents_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_hall/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInsideFaceVents_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInsideFaceVents_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_face_vents/scene.scene.lz"
	.balign 4, 0
