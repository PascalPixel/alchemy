	.global Field_JupiterLighthouseEntrance
Field_JupiterLighthouseEntrance:
	map_header 0, 0, 88, 104, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseEntrance, .LField_JupiterLighthouseEntrance_CELL_PICTURES
	map_block Field_JupiterLighthouseEntrance, .LField_JupiterLighthouseEntrance_SHAPES
	map_block Field_JupiterLighthouseEntrance, .LField_JupiterLighthouseEntrance_GRID
	map_block Field_JupiterLighthouseEntrance, .LField_JupiterLighthouseEntrance_SHAPE_GRID
	map_block Field_JupiterLighthouseEntrance, .LField_JupiterLighthouseEntrance_ANIMATION
	map_block Field_JupiterLighthouseEntrance
	map_block Field_JupiterLighthouseEntrance, .LField_JupiterLighthouseEntrance_SCENE
.LField_JupiterLighthouseEntrance_CELL_PICTURES:
	.incbin "games/tla/src/map/jupiter_lighthouse_entrance/cell_pictures.delta2.lz"
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
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseEntrance_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_entrance/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set103Palette
Tileset_Set103Palette:
	.incbin "games/tla/src/graphics/tileset/set_103/palette.gbapal.lz"
	.global Tileset_Set103TilesA
Tileset_Set103TilesA:
	.incbin "games/tla/src/graphics/tileset/set_103/tiles_a.4bpp.mtf"
	.global Tileset_Set103TilesB
Tileset_Set103TilesB:
	.incbin "games/tla/src/graphics/tileset/set_103/tiles_b.4bpp.mtf"
	.global Tileset_Set103TilesC
Tileset_Set103TilesC:
	.incbin "games/tla/src/graphics/tileset/set_103/tiles_c.4bpp.mtf"
	.global Tileset_Set103TilesD
Tileset_Set103TilesD:
	.incbin "games/tla/src/graphics/tileset/set_103/tiles_d.4bpp.mtf"
	.byte 0x44, 0x16	@ PACKER: left in the buffer by Tileset_Set103TilesC+0xac2
