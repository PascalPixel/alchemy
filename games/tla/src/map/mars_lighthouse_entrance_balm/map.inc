	.global Field_MarsLighthouseEntranceBalm
Field_MarsLighthouseEntranceBalm:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseEntranceBalm, .LField_MarsLighthouseEntranceBalm_CELL_TILES
	map_block Field_MarsLighthouseEntranceBalm, .LField_MarsLighthouseEntranceBalm_SHAPES
	map_block Field_MarsLighthouseEntranceBalm, .LField_MarsLighthouseEntranceBalm_GRID
	map_block Field_MarsLighthouseEntranceBalm, .LField_MarsLighthouseEntranceBalm_SHAPE_GRID
	map_block Field_MarsLighthouseEntranceBalm, .LField_MarsLighthouseEntranceBalm_ANIMATION
	map_block Field_MarsLighthouseEntranceBalm, .LField_MarsLighthouseEntranceBalm_BLEND
	map_block Field_MarsLighthouseEntranceBalm, .LField_MarsLighthouseEntranceBalm_SCENE
.LField_MarsLighthouseEntranceBalm_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_balm/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceBalm_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_balm/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceBalm_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_balm/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceBalm_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_balm/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceBalm_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceBalm_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceBalm_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_balm/scene.scene.lz"
	.balign 4, 0
