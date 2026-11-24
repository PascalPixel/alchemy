	.global Field_JupiterLighthouseTerraces
Field_JupiterLighthouseTerraces:
	map_header 0, 0, 204, 64, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseTerraces, .LField_JupiterLighthouseTerraces_CELL_TILES
	map_block Field_JupiterLighthouseTerraces, .LField_JupiterLighthouseTerraces_SHAPES
	map_block Field_JupiterLighthouseTerraces, .LField_JupiterLighthouseTerraces_GRID
	map_block Field_JupiterLighthouseTerraces, .LField_JupiterLighthouseTerraces_SHAPE_GRID
	map_block Field_JupiterLighthouseTerraces, .LField_JupiterLighthouseTerraces_ANIMATION
	map_block Field_JupiterLighthouseTerraces
	map_block Field_JupiterLighthouseTerraces, .LField_JupiterLighthouseTerraces_SCENE
.LField_JupiterLighthouseTerraces_CELL_TILES:
	.incbin "games/tla/src/map/jupiter_lighthouse_terraces/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_JupiterLighthouseTerraces_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_terraces/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseTerraces_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_terraces/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseTerraces_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_terraces/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseTerraces_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseTerraces_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_terraces/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteJupiterLighthouseTerraces
Tileset_PaletteJupiterLighthouseTerraces:
	.incbin "games/tla/src/graphics/tileset/palettes/jupiter_lighthouse_terraces.gbapal.lz"
	.byte 0x34, 0x61, 0x91	@ PACKER: left in the buffer by Field_JupiterLighthouseTerraces+0x13d
