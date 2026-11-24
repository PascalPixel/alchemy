	.global Field_PsynergyTest
Field_PsynergyTest:
	map_header 0, 0, 64, 32, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 16, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 32, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_PsynergyTest, .LField_PsynergyTest_CELL_TILES
	map_block Field_PsynergyTest, .LField_PsynergyTest_SHAPES
	map_block Field_PsynergyTest, .LField_PsynergyTest_GRID
	map_block Field_PsynergyTest, .LField_PsynergyTest_SHAPE_GRID
	map_block Field_PsynergyTest, .LField_PsynergyTest_ANIMATION
	map_block Field_PsynergyTest, .LField_PsynergyTest_BLEND
	map_block Field_PsynergyTest, .LField_PsynergyTest_SCENE
.LField_PsynergyTest_CELL_TILES:
	.incbin "games/tla/src/map/psynergy_test/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_PsynergyTest_SHAPES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_orihalcon_chest/shapes.table.lz"
	.balign 4, 0
.LField_PsynergyTest_GRID:
	.incbin "games/tla/src/map/psynergy_test/map.grid.lz"
	.balign 4, 0
.LField_PsynergyTest_SHAPE_GRID:
	.incbin "games/tla/src/map/psynergy_test/shape_grid.plane.lz"
	.balign 4, 0
.LField_PsynergyTest_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_PsynergyTest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_PsynergyTest_SCENE:
	.incbin "games/tla/src/map/empty_map/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PsynergyTestPalette
Tileset_PsynergyTestPalette:
	.incbin "games/tla/src/graphics/tileset/psynergy_test/palette.gbapal.lz"
	.global Tileset_PsynergyTestTilesA
Tileset_PsynergyTestTilesA:
	.incbin "games/tla/src/graphics/tileset/psynergy_test/tiles_a.4bpp.mtf"
	.global Tileset_PsynergyTestTilesB
Tileset_PsynergyTestTilesB:
	.incbin "games/tla/src/graphics/tileset/psynergy_test/tiles_b.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Tileset_PsynergyTestTilesA+0x13f
	.global Tileset_PsynergyTestTilesC
Tileset_PsynergyTestTilesC:
	.incbin "games/tla/src/graphics/tileset/psynergy_test/tiles_c.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Tileset_PsynergyTestTilesB+0x13f
	.global Tileset_PsynergyTestTilesD
Tileset_PsynergyTestTilesD:
	.incbin "games/tla/src/graphics/tileset/psynergy_test/tiles_d.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Tileset_PsynergyTestTilesC+0x13f
