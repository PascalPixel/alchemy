	.global Field_AquaRockInsideCrystalPowderChest
Field_AquaRockInsideCrystalPowderChest:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInsideCrystalPowderChest, .LField_AquaRockInsideCrystalPowderChest_CELL_TILES
	map_block Field_AquaRockInsideCrystalPowderChest, .LField_AquaRockInsideCrystalPowderChest_SHAPES
	map_block Field_AquaRockInsideCrystalPowderChest, .LField_AquaRockInsideCrystalPowderChest_GRID
	map_block Field_AquaRockInsideCrystalPowderChest, .LField_AquaRockInsideCrystalPowderChest_SHAPE_GRID
	map_block Field_AquaRockInsideCrystalPowderChest, .LField_AquaRockInsideCrystalPowderChest_ANIMATION
	map_block Field_AquaRockInsideCrystalPowderChest, .LField_AquaRockInsideCrystalPowderChest_BLEND
	map_block Field_AquaRockInsideCrystalPowderChest, .LField_AquaRockInsideCrystalPowderChest_SCENE
.LField_AquaRockInsideCrystalPowderChest_CELL_TILES:
	.incbin "games/tla/src/map/aqua_rock_inside_crystal_powder_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AquaRockInsideCrystalPowderChest_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_crystal_powder_chest/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInsideCrystalPowderChest_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_crystal_powder_chest/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInsideCrystalPowderChest_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_crystal_powder_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInsideCrystalPowderChest_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInsideCrystalPowderChest_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInsideCrystalPowderChest_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_crystal_powder_chest/scene.scene.lz"
