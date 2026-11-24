	.global Field_AirsRockInsideCoinsChest
Field_AirsRockInsideCoinsChest:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInsideCoinsChest, .LField_AirsRockInsideCoinsChest_CELL_TILES
	map_block Field_AirsRockInsideCoinsChest, .LField_AirsRockInsideCoinsChest_SHAPES
	map_block Field_AirsRockInsideCoinsChest, .LField_AirsRockInsideCoinsChest_GRID
	map_block Field_AirsRockInsideCoinsChest, .LField_AirsRockInsideCoinsChest_SHAPE_GRID
	map_block Field_AirsRockInsideCoinsChest, .LField_AirsRockInsideCoinsChest_ANIMATION
	map_block Field_AirsRockInsideCoinsChest, .LField_AirsRockInsideCoinsChest_BLEND
	map_block Field_AirsRockInsideCoinsChest, .LField_AirsRockInsideCoinsChest_SCENE
.LField_AirsRockInsideCoinsChest_CELL_TILES:
	.incbin "games/tla/src/map/airs_rock_inside_coins_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AirsRockInsideCoinsChest_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_coins_chest/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInsideCoinsChest_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_coins_chest/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInsideCoinsChest_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_coins_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInsideCoinsChest_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_hall/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInsideCoinsChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInsideCoinsChest_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_coins_chest/scene.scene.lz"
	.balign 4, 0
