	.global Field_JupiterLighthouseWhorl
Field_JupiterLighthouseWhorl:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseWhorl, .LField_JupiterLighthouseWhorl_CELL_TILES
	map_block Field_JupiterLighthouseWhorl, .LField_JupiterLighthouseWhorl_SHAPES
	map_block Field_JupiterLighthouseWhorl, .LField_JupiterLighthouseWhorl_GRID
	map_block Field_JupiterLighthouseWhorl, .LField_JupiterLighthouseWhorl_SHAPE_GRID
	map_block Field_JupiterLighthouseWhorl, .LField_JupiterLighthouseWhorl_ANIMATION
	map_block Field_JupiterLighthouseWhorl
	map_block Field_JupiterLighthouseWhorl, .LField_JupiterLighthouseWhorl_SCENE
.LField_JupiterLighthouseWhorl_CELL_TILES:
	.incbin "games/tla/src/map/jupiter_lighthouse_whorl/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_JupiterLighthouseWhorl_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_whorl/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseWhorl_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_whorl/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseWhorl_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_whorl/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseWhorl_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseWhorl_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_whorl/scene.scene.lz"
	.balign 4, 0
