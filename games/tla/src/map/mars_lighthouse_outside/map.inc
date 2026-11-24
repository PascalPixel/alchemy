	.global Field_MarsLighthouseOutside
Field_MarsLighthouseOutside:
	map_header 0, 0, 88, 104, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseOutside, .LField_MarsLighthouseOutside_CELL_TILES
	map_block Field_MarsLighthouseOutside, .LField_MarsLighthouseOutside_SHAPES
	map_block Field_MarsLighthouseOutside, .LField_MarsLighthouseOutside_GRID
	map_block Field_MarsLighthouseOutside, .LField_MarsLighthouseOutside_SHAPE_GRID
	map_block Field_MarsLighthouseOutside, .LField_MarsLighthouseOutside_ANIMATION
	map_block Field_MarsLighthouseOutside
	map_block Field_MarsLighthouseOutside, .LField_MarsLighthouseOutside_SCENE
.LField_MarsLighthouseOutside_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_outside/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MarsLighthouseOutside_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_outside/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseOutside_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_outside/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseOutside_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_outside/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseOutside_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseOutside_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_outside/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MarsLighthouseOutsidePalette
Tileset_MarsLighthouseOutsidePalette:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_outside/palette.gbapal.lz"
	.global Tileset_MarsLighthouseOutsideTilesA
Tileset_MarsLighthouseOutsideTilesA:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_outside/tiles_a.4bpp.mtf"
	.byte 0x40, 0xff	@ PACKER: left in the buffer by Field_MarsLighthouseOutside+0xac2
