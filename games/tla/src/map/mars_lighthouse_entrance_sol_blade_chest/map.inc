	.global Field_MarsLighthouseEntranceSolBladeChest
Field_MarsLighthouseEntranceSolBladeChest:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseEntranceSolBladeChest, .LField_MarsLighthouseEntranceSolBladeChest_CELL_TILES
	map_block Field_MarsLighthouseEntranceSolBladeChest, .LField_MarsLighthouseEntranceSolBladeChest_SHAPES
	map_block Field_MarsLighthouseEntranceSolBladeChest, .LField_MarsLighthouseEntranceSolBladeChest_GRID
	map_block Field_MarsLighthouseEntranceSolBladeChest, .LField_MarsLighthouseEntranceSolBladeChest_SHAPE_GRID
	map_block Field_MarsLighthouseEntranceSolBladeChest, .LField_MarsLighthouseEntranceSolBladeChest_ANIMATION
	map_block Field_MarsLighthouseEntranceSolBladeChest, .LField_MarsLighthouseEntranceSolBladeChest_BLEND
	map_block Field_MarsLighthouseEntranceSolBladeChest, .LField_MarsLighthouseEntranceSolBladeChest_SCENE
.LField_MarsLighthouseEntranceSolBladeChest_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_sol_blade_chest/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceSolBladeChest_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_sol_blade_chest/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceSolBladeChest_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_sol_blade_chest/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceSolBladeChest_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_sol_blade_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceSolBladeChest_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceSolBladeChest_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceSolBladeChest_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_sol_blade_chest/scene.scene.lz"
	.balign 4, 0
