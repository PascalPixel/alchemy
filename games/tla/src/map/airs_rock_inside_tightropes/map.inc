	.global Field_AirsRockInsideTightropes
Field_AirsRockInsideTightropes:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInsideTightropes, .LField_AirsRockInsideTightropes_CELL_TILES
	map_block Field_AirsRockInsideTightropes, .LField_AirsRockInsideTightropes_SHAPES
	map_block Field_AirsRockInsideTightropes, .LField_AirsRockInsideTightropes_GRID
	map_block Field_AirsRockInsideTightropes, .LField_AirsRockInsideTightropes_SHAPE_GRID
	map_block Field_AirsRockInsideTightropes, .LField_AirsRockInsideTightropes_ANIMATION
	map_block Field_AirsRockInsideTightropes, .LField_AirsRockInsideTightropes_BLEND
	map_block Field_AirsRockInsideTightropes, .LField_AirsRockInsideTightropes_SCENE
.LField_AirsRockInsideTightropes_CELL_TILES:
	.incbin "games/tla/src/map/airs_rock_inside_tightropes/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AirsRockInsideTightropes_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_tightropes/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInsideTightropes_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_tightropes/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInsideTightropes_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_tightropes/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInsideTightropes_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_hall/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInsideTightropes_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInsideTightropes_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_tightropes/scene.scene.lz"
