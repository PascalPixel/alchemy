	.global Field_AquaRockInsideWaterOfLifeChest
Field_AquaRockInsideWaterOfLifeChest:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInsideWaterOfLifeChest, .LField_AquaRockInsideWaterOfLifeChest_CELL_TILES
	map_block Field_AquaRockInsideWaterOfLifeChest, .LField_AquaRockInsideWaterOfLifeChest_SHAPES
	map_block Field_AquaRockInsideWaterOfLifeChest, .LField_AquaRockInsideWaterOfLifeChest_GRID
	map_block Field_AquaRockInsideWaterOfLifeChest, .LField_AquaRockInsideWaterOfLifeChest_SHAPE_GRID
	map_block Field_AquaRockInsideWaterOfLifeChest, .LField_AquaRockInsideWaterOfLifeChest_ANIMATION
	map_block Field_AquaRockInsideWaterOfLifeChest, .LField_AquaRockInsideWaterOfLifeChest_BLEND
	map_block Field_AquaRockInsideWaterOfLifeChest, .LField_AquaRockInsideWaterOfLifeChest_SCENE
.LField_AquaRockInsideWaterOfLifeChest_CELL_TILES:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AquaRockInsideWaterOfLifeChest_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInsideWaterOfLifeChest_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInsideWaterOfLifeChest_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInsideWaterOfLifeChest_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInsideWaterOfLifeChest_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInsideWaterOfLifeChest_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AquaRockInsideWaterOfLifeChestPalette
Tileset_AquaRockInsideWaterOfLifeChestPalette:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_inside_water_of_life_chest/palette.gbapal.lz"
	.global Tileset_AquaRockInsideWaterOfLifeChestTilesA
Tileset_AquaRockInsideWaterOfLifeChestTilesA:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_inside_water_of_life_chest/tiles_a.4bpp.mtf"
	.byte 0xbd	@ PACKER: left in the buffer by Field_AquaRock2+0x217b
	.global Tileset_AquaRockInsideWaterOfLifeChestTilesB
Tileset_AquaRockInsideWaterOfLifeChestTilesB:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_inside_water_of_life_chest/tiles_b.4bpp.mtf"
	.byte 0x3a	@ PACKER: left in the buffer by Tileset_AquaRockInsideWaterOfLifeChestTilesA+0x1be3
	.global Tileset_AquaRockInsideWaterOfLifeChestTilesC
Tileset_AquaRockInsideWaterOfLifeChestTilesC:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_inside_water_of_life_chest/tiles_c.4bpp.mtf"
	.byte 0x05, 0xb0, 0x73	@ PACKER: left in the buffer by Tileset_AquaRockInsideWaterOfLifeChestTilesA+0x1cf5
	.global Tileset_AquaRockInsideWaterOfLifeChestTilesD
Tileset_AquaRockInsideWaterOfLifeChestTilesD:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_inside_water_of_life_chest/tiles_d.4bpp.mtf"
