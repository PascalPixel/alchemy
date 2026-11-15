	.global Field_TrialRoad3
Field_TrialRoad3:
	map_header 0, 0, 96, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TrialRoad3, .LField_TrialRoad3_CELL_PICTURES
	map_block Field_TrialRoad3, .LField_TrialRoad3_SHAPES
	map_block Field_TrialRoad3, .LField_TrialRoad3_GRID
	map_block Field_TrialRoad3, .LField_TrialRoad3_SHAPE_GRID
	map_block Field_TrialRoad3, .LField_TrialRoad3_ANIMATION
	map_block Field_TrialRoad3, .LField_TrialRoad3_BLEND
	map_block Field_TrialRoad3, .LField_TrialRoad3_SCENE
.LField_TrialRoad3_CELL_PICTURES:
	.incbin "games/tla/src/map/trial_road_3/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_TrialRoad3_SHAPES:
	.incbin "games/tla/src/map/trial_road_3/shapes.table.lz"
	.balign 4, 0
.LField_TrialRoad3_GRID:
	.incbin "games/tla/src/map/trial_road_3/map.grid.lz"
	.balign 4, 0
.LField_TrialRoad3_SHAPE_GRID:
	.incbin "games/tla/src/map/trial_road_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_TrialRoad3_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_TrialRoad3_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TrialRoad3_SCENE:
	.incbin "games/tla/src/map/trial_road_3/scene.scene.lz"
	.balign 4, 0
