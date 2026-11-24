	.global Field_AquaRockInsideVialChest
Field_AquaRockInsideVialChest:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInsideVialChest, .LField_AquaRockInsideVialChest_CELL_TILES
	map_block Field_AquaRockInsideVialChest, .LField_AquaRockInsideVialChest_SHAPES
	map_block Field_AquaRockInsideVialChest, .LField_AquaRockInsideVialChest_GRID
	map_block Field_AquaRockInsideVialChest, .LField_AquaRockInsideVialChest_SHAPE_GRID
	map_block Field_AquaRockInsideVialChest, .LField_AquaRockInsideVialChest_ANIMATION
	map_block Field_AquaRockInsideVialChest, .LField_AquaRockInsideVialChest_BLEND
	map_block Field_AquaRockInsideVialChest, .LField_AquaRockInsideVialChest_SCENE
.LField_AquaRockInsideVialChest_CELL_TILES:
	.incbin "games/tla/src/map/aqua_rock_inside_vial_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AquaRockInsideVialChest_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_vial_chest/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInsideVialChest_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_vial_chest/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInsideVialChest_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_vial_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInsideVialChest_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_vial_chest/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInsideVialChest_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInsideVialChest_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_vial_chest/scene.scene.lz"
	.balign 4, 0
