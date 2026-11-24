	.global Field_MarsLighthouseJupiterTower
Field_MarsLighthouseJupiterTower:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseJupiterTower, .LField_MarsLighthouseJupiterTower_CELL_TILES
	map_block Field_MarsLighthouseJupiterTower, .LField_MarsLighthouseJupiterTower_SHAPES
	map_block Field_MarsLighthouseJupiterTower, .LField_MarsLighthouseJupiterTower_GRID
	map_block Field_MarsLighthouseJupiterTower, .LField_MarsLighthouseJupiterTower_SHAPE_GRID
	map_block Field_MarsLighthouseJupiterTower, .LField_MarsLighthouseJupiterTower_ANIMATION
	map_block Field_MarsLighthouseJupiterTower, .LField_MarsLighthouseJupiterTower_BLEND
	map_block Field_MarsLighthouseJupiterTower, .LField_MarsLighthouseJupiterTower_SCENE
.LField_MarsLighthouseJupiterTower_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_jupiter_tower/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseJupiterTower_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_jupiter_tower/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseJupiterTower_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_jupiter_tower/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_jupiter_tower/map.grid.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseJupiterTower_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_jupiter_tower/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_jupiter_tower/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseJupiterTower_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseJupiterTower_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseJupiterTower_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_jupiter_tower/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_jupiter_tower/scene.scene.lz"
	.endif
	.balign 4, 0
