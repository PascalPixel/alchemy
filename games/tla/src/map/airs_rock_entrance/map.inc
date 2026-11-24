	.global Field_AirsRockEntrance
Field_AirsRockEntrance:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockEntrance, .LField_AirsRockEntrance_CELL_TILES
	map_block Field_AirsRockEntrance, .LField_AirsRockEntrance_SHAPES
	map_block Field_AirsRockEntrance, .LField_AirsRockEntrance_GRID
	map_block Field_AirsRockEntrance, .LField_AirsRockEntrance_SHAPE_GRID
	map_block Field_AirsRockEntrance, .LField_AirsRockEntrance_ANIMATION
	map_block Field_AirsRockEntrance, .LField_AirsRockEntrance_BLEND
	map_block Field_AirsRockEntrance, .LField_AirsRockEntrance_SCENE
.LField_AirsRockEntrance_CELL_TILES:
	.incbin "games/tla/src/map/airs_rock_entrance/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AirsRockEntrance_SHAPES:
	.incbin "games/tla/src/map/airs_rock_entrance/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockEntrance_GRID:
	.incbin "games/tla/src/map/airs_rock_entrance/map.grid.lz"
	.balign 4, 0
.LField_AirsRockEntrance_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_entrance/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockEntrance_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_entrance/animation.script.lz"
	.balign 4, 0
.LField_AirsRockEntrance_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockEntrance_SCENE:
	.incbin "games/tla/src/map/airs_rock_entrance/scene.scene.lz"
	.balign 4, 0
