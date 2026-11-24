	.global Field_MarsLighthouseEntranceHiddenCircle
Field_MarsLighthouseEntranceHiddenCircle:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseEntranceHiddenCircle, .LField_MarsLighthouseEntranceHiddenCircle_CELL_TILES
	map_block Field_MarsLighthouseEntranceHiddenCircle, .LField_MarsLighthouseEntranceHiddenCircle_SHAPES
	map_block Field_MarsLighthouseEntranceHiddenCircle, .LField_MarsLighthouseEntranceHiddenCircle_GRID
	map_block Field_MarsLighthouseEntranceHiddenCircle, .LField_MarsLighthouseEntranceHiddenCircle_SHAPE_GRID
	map_block Field_MarsLighthouseEntranceHiddenCircle, .LField_MarsLighthouseEntranceHiddenCircle_ANIMATION
	map_block Field_MarsLighthouseEntranceHiddenCircle, .LField_MarsLighthouseEntranceHiddenCircle_BLEND
	map_block Field_MarsLighthouseEntranceHiddenCircle, .LField_MarsLighthouseEntranceHiddenCircle_SCENE
.LField_MarsLighthouseEntranceHiddenCircle_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_hidden_circle/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceHiddenCircle_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_hidden_circle/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceHiddenCircle_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_hidden_circle/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceHiddenCircle_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_hidden_circle/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceHiddenCircle_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceHiddenCircle_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseEntranceHiddenCircle_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_entrance_hidden_circle/scene.scene.lz"
