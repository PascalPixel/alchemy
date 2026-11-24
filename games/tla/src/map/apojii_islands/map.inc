	.global Field_ApojiiIslands
Field_ApojiiIslands:
	map_header 0, 0, 88, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ApojiiIslands, .LField_ApojiiIslands_CELL_TILES
	map_block Field_ApojiiIslands, .LField_ApojiiIslands_SHAPES
	map_block Field_ApojiiIslands, .LField_ApojiiIslands_GRID
	map_block Field_ApojiiIslands, .LField_ApojiiIslands_SHAPE_GRID
	map_block Field_ApojiiIslands, .LField_ApojiiIslands_ANIMATION
	map_block Field_ApojiiIslands, .LField_ApojiiIslands_BLEND
	map_block Field_ApojiiIslands, .LField_ApojiiIslands_SCENE
.LField_ApojiiIslands_CELL_TILES:
	.incbin "games/tla/src/map/apojii_islands/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ApojiiIslands_SHAPES:
	.incbin "games/tla/src/map/apojii_islands/shapes.table.lz"
	.balign 4, 0
.LField_ApojiiIslands_GRID:
	.incbin "games/tla/src/map/apojii_islands/map.grid.lz"
	.balign 4, 0
.LField_ApojiiIslands_SHAPE_GRID:
	.incbin "games/tla/src/map/apojii_islands/shape_grid.plane.lz"
	.balign 4, 0
.LField_ApojiiIslands_ANIMATION:
	.incbin "games/tla/src/map/apojii_islands/animation.script.lz"
	.balign 4, 0
.LField_ApojiiIslands_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ApojiiIslands_SCENE:
	.incbin "games/tla/src/map/apojii_islands/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ApojiiIslandsPalette
Tileset_ApojiiIslandsPalette:
	.incbin "games/tla/src/graphics/tileset/apojii_islands/palette.gbapal.lz"
	.byte 0x78, 0x9f	@ PACKER: left in the buffer by Field_ApojiiIslands+0x156
	.global Tileset_ApojiiIslandsTilesA
Tileset_ApojiiIslandsTilesA:
	.incbin "games/tla/src/graphics/tileset/apojii_islands/tiles_a.4bpp.mtf"
	.global Tileset_ApojiiIslandsTilesB
Tileset_ApojiiIslandsTilesB:
	.incbin "games/tla/src/graphics/tileset/apojii_islands/tiles_b.4bpp.mtf"
	.byte 0xa3	@ PACKER: left in the buffer by Tileset_ApojiiIslandsTilesA+0x1ab7
	.global Tileset_ApojiiIslandsTilesC
Tileset_ApojiiIslandsTilesC:
	.incbin "games/tla/src/graphics/tileset/apojii_islands/tiles_c.4bpp.mtf"
	.byte 0x0f, 0x74, 0xf6	@ PACKER: left in the buffer by Tileset_ApojiiIslandsTilesB+0x1881
	.global Tileset_ApojiiIslandsTilesD
Tileset_ApojiiIslandsTilesD:
	.incbin "games/tla/src/graphics/tileset/apojii_islands/tiles_d.4bpp.mtf"
	.byte 0x6d, 0x93, 0xa2	@ PACKER: left in the buffer by Field_ApojiiIslands+0x1d25
