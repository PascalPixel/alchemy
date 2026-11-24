	.global Field_AirsRockStormBrandChest
Field_AirsRockStormBrandChest:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockStormBrandChest, .LField_AirsRockStormBrandChest_CELL_TILES
	map_block Field_AirsRockStormBrandChest, .LField_AirsRockStormBrandChest_SHAPES
	map_block Field_AirsRockStormBrandChest, .LField_AirsRockStormBrandChest_GRID
	map_block Field_AirsRockStormBrandChest, .LField_AirsRockStormBrandChest_SHAPE_GRID
	map_block Field_AirsRockStormBrandChest, .LField_AirsRockStormBrandChest_ANIMATION
	map_block Field_AirsRockStormBrandChest, .LField_AirsRockStormBrandChest_BLEND
	map_block Field_AirsRockStormBrandChest, .LField_AirsRockStormBrandChest_SCENE
.LField_AirsRockStormBrandChest_CELL_TILES:
	.incbin "games/tla/src/map/airs_rock_storm_brand_chest/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_AirsRockStormBrandChest_SHAPES:
	.incbin "games/tla/src/map/airs_rock_storm_brand_chest/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockStormBrandChest_GRID:
	.incbin "games/tla/src/map/airs_rock_storm_brand_chest/map.grid.lz"
	.balign 4, 0
.LField_AirsRockStormBrandChest_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_storm_brand_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockStormBrandChest_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_AirsRockStormBrandChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockStormBrandChest_SCENE:
	.incbin "games/tla/src/map/airs_rock_storm_brand_chest/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AirsRockStormBrandChestPalette
Tileset_AirsRockStormBrandChestPalette:
	.incbin "games/tla/src/graphics/tileset/airs_rock_storm_brand_chest/palette.gbapal.lz"
	.byte 0x98, 0xb7, 0xe6	@ PACKER: left in the buffer by Field_AirsRock1+0x191
	.global Tileset_AirsRockStormBrandChestTilesA
Tileset_AirsRockStormBrandChestTilesA:
	.incbin "games/tla/src/graphics/tileset/airs_rock_storm_brand_chest/tiles_a.4bpp.mtf"
	.global Tileset_AirsRockStormBrandChestTilesB
Tileset_AirsRockStormBrandChestTilesB:
	.incbin "games/tla/src/graphics/tileset/airs_rock_storm_brand_chest/tiles_b.4bpp.mtf"
	.byte 0x48	@ PACKER: left in the buffer by Tileset_AirsRockStormBrandChestTilesA+0x269f
	.global Tileset_AirsRockStormBrandChestTilesC
Tileset_AirsRockStormBrandChestTilesC:
	.incbin "games/tla/src/graphics/tileset/airs_rock_storm_brand_chest/tiles_c.4bpp.mtf"
	.byte 0xe7, 0xe3, 0x0d	@ PACKER: left in the buffer by Tileset_AirsRockStormBrandChestTilesB+0x2681
	.global Tileset_AirsRockStormBrandChestTilesD
Tileset_AirsRockStormBrandChestTilesD:
	.incbin "games/tla/src/graphics/tileset/airs_rock_storm_brand_chest/tiles_d.4bpp.mtf"
	.byte 0x7b	@ PACKER: left in the buffer by Tileset_AirsRockStormBrandChestTilesC+0x333
