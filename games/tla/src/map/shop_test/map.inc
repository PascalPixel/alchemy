	.global Field_ShopTest
Field_ShopTest:
	map_header 0, 0, 100, 64, 3, 1, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShopTest, .LField_ShopTest_CELL_PICTURES
	map_block Field_ShopTest, .LField_ShopTest_SHAPES
	map_block Field_ShopTest, .LField_ShopTest_GRID
	map_block Field_ShopTest, .LField_ShopTest_SHAPE_GRID
	map_block Field_ShopTest, .LField_ShopTest_ANIMATION
	map_block Field_ShopTest, .LField_ShopTest_BLEND
	map_block Field_ShopTest, .LField_ShopTest_SCENE
.LField_ShopTest_CELL_PICTURES:
	.incbin "games/tla/src/map/shop_test/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ShopTest_SHAPES:
	.incbin "games/tla/src/map/shop_test/shapes.table.lz"
	.balign 4, 0
.LField_ShopTest_GRID:
	.incbin "games/tla/src/map/shop_test/map.grid.lz"
	.balign 4, 0
.LField_ShopTest_SHAPE_GRID:
	.incbin "games/tla/src/map/shop_test/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShopTest_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_ShopTest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShopTest_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ShopTestPalette
Tileset_ShopTestPalette:
	.incbin "games/tla/src/graphics/tileset/shop_test/palette.gbapal.lz"
	.global Tileset_ShopTestTilesA
Tileset_ShopTestTilesA:
	.incbin "games/tla/src/graphics/tileset/shop_test/tiles_a.4bpp.mtf"
	.byte 0x26, 0x00, 0x00	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.global Tileset_ShopTestTilesB
Tileset_ShopTestTilesB:
	.incbin "games/tla/src/graphics/tileset/shop_test/tiles_b.4bpp.mtf"
	.byte 0x0f, 0xd8	@ PACKER: left in the buffer by Tileset_ShopTestTilesA+0x1772
	.global Tileset_ShopTestTilesC
Tileset_ShopTestTilesC:
	.incbin "games/tla/src/graphics/tileset/shop_test/tiles_c.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Tileset_ShopTestTilesB+0x12bb
	.global Tileset_ShopTestTilesD
Tileset_ShopTestTilesD:
	.incbin "games/tla/src/graphics/tileset/set_119/tiles_d.4bpp.mtf"
	.byte 0xe5, 0x3d	@ PACKER: left in the buffer by Tileset_ShopTestTilesC+0x842
