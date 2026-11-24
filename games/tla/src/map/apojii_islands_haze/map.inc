	.global Field_ApojiiIslandsHaze
Field_ApojiiIslandsHaze:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ApojiiIslandsHaze, .LField_ApojiiIslandsHaze_CELL_TILES
	map_block Field_ApojiiIslandsHaze, .LField_ApojiiIslandsHaze_SHAPES
	map_block Field_ApojiiIslandsHaze, .LField_ApojiiIslandsHaze_GRID
	map_block Field_ApojiiIslandsHaze, .LField_ApojiiIslandsHaze_SHAPE_GRID
	map_block Field_ApojiiIslandsHaze, .LField_ApojiiIslandsHaze_ANIMATION
	map_block Field_ApojiiIslandsHaze, .LField_ApojiiIslandsHaze_BLEND
	map_block Field_ApojiiIslandsHaze, .LField_ApojiiIslandsHaze_SCENE
.LField_ApojiiIslandsHaze_CELL_TILES:
	.incbin "games/tla/src/map/apojii_islands_haze/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ApojiiIslandsHaze_SHAPES:
	.incbin "games/tla/src/map/apojii_islands_haze/shapes.table.lz"
	.balign 4, 0
.LField_ApojiiIslandsHaze_GRID:
	.incbin "games/tla/src/map/apojii_islands_haze/map.grid.lz"
	.balign 4, 0
.LField_ApojiiIslandsHaze_SHAPE_GRID:
	.incbin "games/tla/src/map/apojii_islands_haze/shape_grid.plane.lz"
	.balign 4, 0
.LField_ApojiiIslandsHaze_ANIMATION:
	.incbin "games/tla/src/map/apojii_islands_haze/animation.script.lz"
	.balign 4, 0
.LField_ApojiiIslandsHaze_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ApojiiIslandsHaze_SCENE:
	.incbin "games/tla/src/map/apojii_islands_haze/scene.scene.lz"
	.global Tileset_ApojiiIslandsHazePalette
Tileset_ApojiiIslandsHazePalette:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_haze/palette.gbapal.lz"
	.global Tileset_ApojiiIslandsHazeTilesA
Tileset_ApojiiIslandsHazeTilesA:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_haze/tiles_a.4bpp.mtf"
	.byte 0x0d, 0x09	@ PACKER: left in the buffer by Field_ApojiiIslands+0x1e3a
	.global Tileset_ApojiiIslandsHazeTilesB
Tileset_ApojiiIslandsHazeTilesB:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_haze/tiles_b.4bpp.mtf"
	.global Tileset_ApojiiIslandsHazeTilesC
Tileset_ApojiiIslandsHazeTilesC:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_haze/tiles_c.4bpp.mtf"
	.global Tileset_ApojiiIslandsHazeTilesD
Tileset_ApojiiIslandsHazeTilesD:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_haze/tiles_d.4bpp.mtf"
	.byte 0x11	@ PACKER: left in the buffer by Tileset_ApojiiIslandsHazeTilesC+0x195f
