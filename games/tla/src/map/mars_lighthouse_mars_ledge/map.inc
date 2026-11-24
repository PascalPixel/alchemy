	.global Field_MarsLighthouseMarsLedge
Field_MarsLighthouseMarsLedge:
	map_header 0, 0, 204, 64, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseMarsLedge, .LField_MarsLighthouseMarsLedge_CELL_TILES
	map_block Field_MarsLighthouseMarsLedge, .LField_MarsLighthouseMarsLedge_SHAPES
	map_block Field_MarsLighthouseMarsLedge, .LField_MarsLighthouseMarsLedge_GRID
	map_block Field_MarsLighthouseMarsLedge, .LField_MarsLighthouseMarsLedge_SHAPE_GRID
	map_block Field_MarsLighthouseMarsLedge, .LField_MarsLighthouseMarsLedge_ANIMATION
	map_block Field_MarsLighthouseMarsLedge
	map_block Field_MarsLighthouseMarsLedge, .LField_MarsLighthouseMarsLedge_SCENE
.LField_MarsLighthouseMarsLedge_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_mars_ledge/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseMarsLedge_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_mars_ledge/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseMarsLedge_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_mars_ledge/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseMarsLedge_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_mars_ledge/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseMarsLedge_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseMarsLedge_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_mars_ledge/scene.scene.lz"
	.balign 4, 0
