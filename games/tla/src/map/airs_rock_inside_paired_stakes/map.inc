	.global Field_AirsRockInsidePairedStakes
Field_AirsRockInsidePairedStakes:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInsidePairedStakes, .LField_AirsRockInsidePairedStakes_CELL_TILES
	map_block Field_AirsRockInsidePairedStakes, .LField_AirsRockInsidePairedStakes_SHAPES
	map_block Field_AirsRockInsidePairedStakes, .LField_AirsRockInsidePairedStakes_GRID
	map_block Field_AirsRockInsidePairedStakes, .LField_AirsRockInsidePairedStakes_SHAPE_GRID
	map_block Field_AirsRockInsidePairedStakes, .LField_AirsRockInsidePairedStakes_ANIMATION
	map_block Field_AirsRockInsidePairedStakes, .LField_AirsRockInsidePairedStakes_BLEND
	map_block Field_AirsRockInsidePairedStakes, .LField_AirsRockInsidePairedStakes_SCENE
.LField_AirsRockInsidePairedStakes_CELL_TILES:
	.incbin "games/tla/src/map/airs_rock_inside_paired_stakes/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AirsRockInsidePairedStakes_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_paired_stakes/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInsidePairedStakes_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_paired_stakes/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInsidePairedStakes_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_paired_stakes/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInsidePairedStakes_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_hall/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInsidePairedStakes_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInsidePairedStakes_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_paired_stakes/scene.scene.lz"
	.balign 4, 0
