	.global Field_MarsLighthouseMercuryTower
Field_MarsLighthouseMercuryTower:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseMercuryTower, .LField_MarsLighthouseMercuryTower_CELL_TILES
	map_block Field_MarsLighthouseMercuryTower, .LField_MarsLighthouseMercuryTower_SHAPES
	map_block Field_MarsLighthouseMercuryTower, .LField_MarsLighthouseMercuryTower_GRID
	map_block Field_MarsLighthouseMercuryTower, .LField_MarsLighthouseMercuryTower_SHAPE_GRID
	map_block Field_MarsLighthouseMercuryTower, .LField_MarsLighthouseMercuryTower_ANIMATION
	map_block Field_MarsLighthouseMercuryTower, .LField_MarsLighthouseMercuryTower_BLEND
	map_block Field_MarsLighthouseMercuryTower, .LField_MarsLighthouseMercuryTower_SCENE
.LField_MarsLighthouseMercuryTower_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseMercuryTower_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseMercuryTower_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/map.grid.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseMercuryTower_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseMercuryTower_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseMercuryTower_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseMercuryTower_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/scene.scene.lz"
	.endif
