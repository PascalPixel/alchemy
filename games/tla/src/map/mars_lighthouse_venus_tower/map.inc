	.global Field_MarsLighthouseVenusTower
Field_MarsLighthouseVenusTower:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseVenusTower, .LField_MarsLighthouseVenusTower_CELL_TILES
	map_block Field_MarsLighthouseVenusTower, .LField_MarsLighthouseVenusTower_SHAPES
	map_block Field_MarsLighthouseVenusTower, .LField_MarsLighthouseVenusTower_GRID
	map_block Field_MarsLighthouseVenusTower, .LField_MarsLighthouseVenusTower_SHAPE_GRID
	map_block Field_MarsLighthouseVenusTower, .LField_MarsLighthouseVenusTower_ANIMATION
	map_block Field_MarsLighthouseVenusTower, .LField_MarsLighthouseVenusTower_BLEND
	map_block Field_MarsLighthouseVenusTower, .LField_MarsLighthouseVenusTower_SCENE
.LField_MarsLighthouseVenusTower_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_venus_tower/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseVenusTower_SHAPES:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_venus_tower/shapes_ja.table.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_venus_tower/shapes.table.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseVenusTower_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_venus_tower/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_venus_tower/map.grid.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseVenusTower_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_venus_tower/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_venus_tower/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseVenusTower_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseVenusTower_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseVenusTower_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_venus_tower/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_venus_tower/scene.scene.lz"
	.endif
	.balign 4, 0
