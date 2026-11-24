	.global Field_MagmaRockInsideGolemCoreChest
Field_MagmaRockInsideGolemCoreChest:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MagmaRockInsideGolemCoreChest, .LField_MagmaRockInsideGolemCoreChest_CELL_TILES
	map_block Field_MagmaRockInsideGolemCoreChest, .LField_MagmaRockInsideGolemCoreChest_SHAPES
	map_block Field_MagmaRockInsideGolemCoreChest, .LField_MagmaRockInsideGolemCoreChest_GRID
	map_block Field_MagmaRockInsideGolemCoreChest, .LField_MagmaRockInsideGolemCoreChest_SHAPE_GRID
	map_block Field_MagmaRockInsideGolemCoreChest, .LField_MagmaRockInsideGolemCoreChest_ANIMATION
	map_block Field_MagmaRockInsideGolemCoreChest, .LField_MagmaRockInsideGolemCoreChest_BLEND
	map_block Field_MagmaRockInsideGolemCoreChest, .LField_MagmaRockInsideGolemCoreChest_SCENE
.LField_MagmaRockInsideGolemCoreChest_CELL_TILES:
	.incbin "games/tla/src/map/magma_rock_inside_golem_core_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MagmaRockInsideGolemCoreChest_SHAPES:
	.incbin "games/tla/src/map/magma_rock_inside_golem_core_chest/shapes.table.lz"
	.balign 4, 0
.LField_MagmaRockInsideGolemCoreChest_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_golem_core_chest/map.grid.lz"
	.balign 4, 0
.LField_MagmaRockInsideGolemCoreChest_SHAPE_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_golem_core_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_MagmaRockInsideGolemCoreChest_ANIMATION:
	.incbin "games/tla/src/map/magma_rock_inside_entrance/animation.script.lz"
	.balign 4, 0
.LField_MagmaRockInsideGolemCoreChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MagmaRockInsideGolemCoreChest_SCENE:
	.incbin "games/tla/src/map/magma_rock_inside_golem_core_chest/scene.scene.lz"
	.balign 4, 0
