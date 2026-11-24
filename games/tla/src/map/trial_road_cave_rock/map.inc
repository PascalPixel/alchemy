	.global Field_TrialRoadCaveRock
Field_TrialRoadCaveRock:
	map_header 0, 0, 96, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TrialRoadCaveRock, .LField_TrialRoadCaveRock_CELL_TILES
	map_block Field_TrialRoadCaveRock, .LField_TrialRoadCaveRock_SHAPES
	map_block Field_TrialRoadCaveRock, .LField_TrialRoadCaveRock_GRID
	map_block Field_TrialRoadCaveRock, .LField_TrialRoadCaveRock_SHAPE_GRID
	map_block Field_TrialRoadCaveRock, .LField_TrialRoadCaveRock_ANIMATION
	map_block Field_TrialRoadCaveRock, .LField_TrialRoadCaveRock_BLEND
	map_block Field_TrialRoadCaveRock, .LField_TrialRoadCaveRock_SCENE
.LField_TrialRoadCaveRock_CELL_TILES:
	.incbin "games/tla/src/map/trial_road_cave_rock/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_TrialRoadCaveRock_SHAPES:
	.incbin "games/tla/src/map/trial_road_cave_rock/shapes.table.lz"
	.balign 4, 0
.LField_TrialRoadCaveRock_GRID:
	.incbin "games/tla/src/map/trial_road_cave_rock/map.grid.lz"
	.balign 4, 0
.LField_TrialRoadCaveRock_SHAPE_GRID:
	.incbin "games/tla/src/map/trial_road_cave_rock/shape_grid.plane.lz"
	.balign 4, 0
.LField_TrialRoadCaveRock_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_TrialRoadCaveRock_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TrialRoadCaveRock_SCENE:
	.incbin "games/tla/src/map/trial_road_cave_rock/scene.scene.lz"
	.balign 4, 0
