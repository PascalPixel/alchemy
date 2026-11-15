	.global Field_JupiterLighthouseBasement2
Field_JupiterLighthouseBasement2:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseBasement2, .LField_JupiterLighthouseBasement2_CELL_PICTURES
	map_block Field_JupiterLighthouseBasement2, .LField_JupiterLighthouseBasement2_SHAPES
	map_block Field_JupiterLighthouseBasement2, .LField_JupiterLighthouseBasement2_GRID
	map_block Field_JupiterLighthouseBasement2, .LField_JupiterLighthouseBasement2_SHAPE_GRID
	map_block Field_JupiterLighthouseBasement2, .LField_JupiterLighthouseBasement2_ANIMATION
	map_block Field_JupiterLighthouseBasement2, .LField_JupiterLighthouseBasement2_BLEND
	map_block Field_JupiterLighthouseBasement2, .LField_JupiterLighthouseBasement2_SCENE
.LField_JupiterLighthouseBasement2_CELL_PICTURES:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasement2_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_2/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasement2_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_2/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasement2_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasement2_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasement2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasement2_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_2/scene.scene.lz"
