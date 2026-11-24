	.global Field_MarsLighthouseEntranceFugue
Field_MarsLighthouseEntranceFugue:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseEntranceFugue, .LField_MarsLighthouseEntranceFugue_CELL_TILES
	map_block Field_MarsLighthouseEntranceFugue, .LField_MarsLighthouseEntranceFugue_SHAPES
	map_block Field_MarsLighthouseEntranceFugue, .LField_MarsLighthouseEntranceFugue_GRID
	map_block Field_MarsLighthouseEntranceFugue, .LField_MarsLighthouseEntranceFugue_SHAPE_GRID
	map_block Field_MarsLighthouseEntranceFugue, .LField_MarsLighthouseEntranceFugue_ANIMATION
	map_block Field_MarsLighthouseEntranceFugue, .LField_MarsLighthouseEntranceFugue_BLEND
	map_block Field_MarsLighthouseEntranceFugue, .LField_MarsLighthouseEntranceFugue_SCENE
.LField_MarsLighthouseEntranceFugue_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_fugue/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceFugue_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_fugue/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceFugue_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_entrance_fugue/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_entrance_fugue/map.grid.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseEntranceFugue_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_entrance_fugue/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_entrance_fugue/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_MarsLighthouseEntranceFugue_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceFugue_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceFugue_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/mars_lighthouse_entrance_fugue/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/mars_lighthouse_entrance_fugue/scene.scene.lz"
	.endif
