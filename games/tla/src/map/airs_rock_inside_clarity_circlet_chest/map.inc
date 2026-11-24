	.global Field_AirsRockInsideClarityCircletChest
Field_AirsRockInsideClarityCircletChest:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInsideClarityCircletChest, .LField_AirsRockInsideClarityCircletChest_CELL_TILES
	map_block Field_AirsRockInsideClarityCircletChest, .LField_AirsRockInsideClarityCircletChest_SHAPES
	map_block Field_AirsRockInsideClarityCircletChest, .LField_AirsRockInsideClarityCircletChest_GRID
	map_block Field_AirsRockInsideClarityCircletChest, .LField_AirsRockInsideClarityCircletChest_SHAPE_GRID
	map_block Field_AirsRockInsideClarityCircletChest, .LField_AirsRockInsideClarityCircletChest_ANIMATION
	map_block Field_AirsRockInsideClarityCircletChest, .LField_AirsRockInsideClarityCircletChest_BLEND
	map_block Field_AirsRockInsideClarityCircletChest, .LField_AirsRockInsideClarityCircletChest_SCENE
.LField_AirsRockInsideClarityCircletChest_CELL_TILES:
	.incbin "games/tla/src/map/airs_rock_inside_clarity_circlet_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AirsRockInsideClarityCircletChest_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_clarity_circlet_chest/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInsideClarityCircletChest_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_clarity_circlet_chest/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInsideClarityCircletChest_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_clarity_circlet_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInsideClarityCircletChest_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_hall/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInsideClarityCircletChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInsideClarityCircletChest_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_clarity_circlet_chest/scene.scene.lz"
	.balign 4, 0
