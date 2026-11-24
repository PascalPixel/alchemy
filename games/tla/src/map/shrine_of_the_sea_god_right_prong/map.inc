	.global Field_ShrineOfTheSeaGodRightProng
Field_ShrineOfTheSeaGodRightProng:
	map_header 0, 0, 128, 128, 2, 2, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 12, 12, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGodRightProng, .LField_ShrineOfTheSeaGodRightProng_CELL_TILES
	map_block Field_ShrineOfTheSeaGodRightProng, .LField_ShrineOfTheSeaGodRightProng_SHAPES
	map_block Field_ShrineOfTheSeaGodRightProng, .LField_ShrineOfTheSeaGodRightProng_GRID
	map_block Field_ShrineOfTheSeaGodRightProng, .LField_ShrineOfTheSeaGodRightProng_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGodRightProng, .LField_ShrineOfTheSeaGodRightProng_ANIMATION
	map_block Field_ShrineOfTheSeaGodRightProng
	map_block Field_ShrineOfTheSeaGodRightProng, .LField_ShrineOfTheSeaGodRightProng_SCENE
.LField_ShrineOfTheSeaGodRightProng_CELL_TILES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_right_prong/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodRightProng_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_right_prong/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodRightProng_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_right_prong/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodRightProng_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_right_prong/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodRightProng_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_right_prong/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodRightProng_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_right_prong/scene.scene.lz"
	.global Tileset_ShrineOfTheSeaGodRightProngPalette
Tileset_ShrineOfTheSeaGodRightProngPalette:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_right_prong/palette.gbapal.lz"
	.byte 0x27, 0x1f	@ PACKER: left in the buffer by Field_ShrineOfTheSeaGod11+0x16e
	.global Tileset_ShrineOfTheSeaGodRightProngTilesA
Tileset_ShrineOfTheSeaGodRightProngTilesA:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_right_prong/tiles_a.4bpp.mtf"
	.byte 0x81, 0x45	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGodSeaGodsTearTilesA+0x259a
	.global Tileset_ShrineOfTheSeaGodRightProngTilesB
Tileset_ShrineOfTheSeaGodRightProngTilesB:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_right_prong/tiles_b.4bpp.mtf"
	.global Tileset_ShrineOfTheSeaGodRightProngTilesC
Tileset_ShrineOfTheSeaGodRightProngTilesC:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_right_prong/tiles_c.4bpp.mtf"
	.byte 0xd8, 0x18	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGodRightProngTilesB+0x1e86
	.global Tileset_ShrineOfTheSeaGodRightProngTilesD
Tileset_ShrineOfTheSeaGodRightProngTilesD:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_right_prong/tiles_d.4bpp.mtf"
	.byte 0x08	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGodRightProngTilesC+0x6c3
