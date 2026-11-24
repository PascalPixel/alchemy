	.global Field_JupiterLighthouseEntrance
Field_JupiterLighthouseEntrance:
	map_header 0, 0, 88, 104, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseEntrance, .LField_JupiterLighthouseEntrance_CELL_TILES
	map_block Field_JupiterLighthouseEntrance, .LField_JupiterLighthouseEntrance_SHAPES
	map_block Field_JupiterLighthouseEntrance, .LField_JupiterLighthouseEntrance_GRID
	map_block Field_JupiterLighthouseEntrance, .LField_JupiterLighthouseEntrance_SHAPE_GRID
	map_block Field_JupiterLighthouseEntrance, .LField_JupiterLighthouseEntrance_ANIMATION
	map_block Field_JupiterLighthouseEntrance
	map_block Field_JupiterLighthouseEntrance, .LField_JupiterLighthouseEntrance_SCENE
.LField_JupiterLighthouseEntrance_CELL_TILES:
	.incbin "games/tla/src/map/jupiter_lighthouse_entrance/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_JupiterLighthouseEntrance_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_entrance/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseEntrance_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_entrance/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseEntrance_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_entrance/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseEntrance_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseEntrance_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_entrance/scene.scene.lz"
	.balign 4, 0
	.global Tileset_JupiterLighthouseEntrancePalette
Tileset_JupiterLighthouseEntrancePalette:
	.incbin "games/tla/src/graphics/tileset/jupiter_lighthouse_entrance/palette.gbapal.lz"
	.global Tileset_JupiterLighthouseEntranceTilesA
Tileset_JupiterLighthouseEntranceTilesA:
	.incbin "games/tla/src/graphics/tileset/jupiter_lighthouse_entrance/tiles_a.4bpp.mtf"
	.global Tileset_JupiterLighthouseEntranceTilesB
Tileset_JupiterLighthouseEntranceTilesB:
	.incbin "games/tla/src/graphics/tileset/jupiter_lighthouse_entrance/tiles_b.4bpp.mtf"
	.global Tileset_JupiterLighthouseEntranceTilesC
Tileset_JupiterLighthouseEntranceTilesC:
	.incbin "games/tla/src/graphics/tileset/jupiter_lighthouse_entrance/tiles_c.4bpp.mtf"
	.global Tileset_JupiterLighthouseEntranceTilesD
Tileset_JupiterLighthouseEntranceTilesD:
	.incbin "games/tla/src/graphics/tileset/jupiter_lighthouse_entrance/tiles_d.4bpp.mtf"
	.byte 0x44, 0x16	@ PACKER: left in the buffer by Tileset_JupiterLighthouseEntranceTilesC+0xac2
