	.global Field_JupiterLighthouseBasementTeleporters
Field_JupiterLighthouseBasementTeleporters:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseBasementTeleporters, .LField_JupiterLighthouseBasementTeleporters_CELL_TILES
	map_block Field_JupiterLighthouseBasementTeleporters, .LField_JupiterLighthouseBasementTeleporters_SHAPES
	map_block Field_JupiterLighthouseBasementTeleporters, .LField_JupiterLighthouseBasementTeleporters_GRID
	map_block Field_JupiterLighthouseBasementTeleporters, .LField_JupiterLighthouseBasementTeleporters_SHAPE_GRID
	map_block Field_JupiterLighthouseBasementTeleporters, .LField_JupiterLighthouseBasementTeleporters_ANIMATION
	map_block Field_JupiterLighthouseBasementTeleporters
	map_block Field_JupiterLighthouseBasementTeleporters, .LField_JupiterLighthouseBasementTeleporters_SCENE
.LField_JupiterLighthouseBasementTeleporters_CELL_TILES:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_teleporters/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasementTeleporters_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_teleporters/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasementTeleporters_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_teleporters/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasementTeleporters_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_teleporters/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasementTeleporters_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseBasementTeleporters_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_basement_teleporters/scene.scene.lz"
	.global Tileset_PaletteJupiterLighthouseBasement1
Tileset_PaletteJupiterLighthouseBasement1:
	.incbin "games/tla/src/graphics/tileset/palettes/jupiter_lighthouse_basement_teleporters.gbapal.lz"
