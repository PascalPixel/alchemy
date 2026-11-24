	.global Field_MarsLighthouseJupiterLedge
Field_MarsLighthouseJupiterLedge:
	map_header 0, 0, 204, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 160, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseJupiterLedge, .LField_MarsLighthouseJupiterLedge_CELL_TILES
	map_block Field_MarsLighthouseJupiterLedge, .LField_MarsLighthouseJupiterLedge_SHAPES
	map_block Field_MarsLighthouseJupiterLedge, .LField_MarsLighthouseJupiterLedge_GRID
	map_block Field_MarsLighthouseJupiterLedge, .LField_MarsLighthouseJupiterLedge_SHAPE_GRID
	map_block Field_MarsLighthouseJupiterLedge, .LField_MarsLighthouseJupiterLedge_ANIMATION
	map_block Field_MarsLighthouseJupiterLedge
	map_block Field_MarsLighthouseJupiterLedge, .LField_MarsLighthouseJupiterLedge_SCENE
.LField_MarsLighthouseJupiterLedge_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_jupiter_ledge/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseJupiterLedge_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_jupiter_ledge/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseJupiterLedge_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_jupiter_ledge/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseJupiterLedge_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_jupiter_ledge/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseJupiterLedge_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseJupiterLedge_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_jupiter_ledge/scene.scene.lz"
	.balign 4, 0
