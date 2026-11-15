	.global Field_JupiterLighthouseBasement1
Field_JupiterLighthouseBasement1:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseBasement1, .LField_JupiterLighthouseBasement1_CELL_PICTURES
	map_block Field_JupiterLighthouseBasement1, .LField_JupiterLighthouseBasement1_SHAPES
	map_block Field_JupiterLighthouseBasement1, .LField_JupiterLighthouseBasement1_GRID
	map_block Field_JupiterLighthouseBasement1, .LField_JupiterLighthouseBasement1_SHAPE_GRID
	map_block Field_JupiterLighthouseBasement1, .LField_JupiterLighthouseBasement1_ANIMATION
	map_block Field_JupiterLighthouseBasement1
	map_block Field_JupiterLighthouseBasement1, .LField_JupiterLighthouseBasement1_SCENE
.LField_JupiterLighthouseBasement1_CELL_PICTURES:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasement1_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_1/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasement1_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_1/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasement1_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasement1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasement1_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_1/scene.scene.lz"
	.global Tileset_PaletteJupiterLighthouseBasement1
Tileset_PaletteJupiterLighthouseBasement1:
	.incbin "games/tla/src/graphics/tileset/palettes/jupiter_lighthouse_basement_1.gbapal.lz"
