	.global Field_JupiterLighthouseAerie
Field_JupiterLighthouseAerie:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseAerie, .LField_JupiterLighthouseAerie_CELL_TILES
	map_block Field_JupiterLighthouseAerie, .LField_JupiterLighthouseAerie_SHAPES
	map_block Field_JupiterLighthouseAerie, .LField_JupiterLighthouseAerie_GRID
	map_block Field_JupiterLighthouseAerie, .LField_JupiterLighthouseAerie_SHAPE_GRID
	map_block Field_JupiterLighthouseAerie, .LField_JupiterLighthouseAerie_ANIMATION
	map_block Field_JupiterLighthouseAerie
	map_block Field_JupiterLighthouseAerie, .LField_JupiterLighthouseAerie_SCENE
.LField_JupiterLighthouseAerie_CELL_TILES:
	.incbin "games/tla/src/map/jupiter_lighthouse_aerie/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_JupiterLighthouseAerie_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_aerie/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseAerie_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_aerie/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseAerie_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_aerie/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseAerie_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseAerie_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_aerie/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteJupiterLighthouseAerie
Tileset_PaletteJupiterLighthouseAerie:
	.incbin "games/tla/src/graphics/tileset/palettes/jupiter_lighthouse_aerie.gbapal.lz"
	.byte 0x4c	@ PACKER: left in the buffer by Field_JupiterLighthouseAerie+0x127
