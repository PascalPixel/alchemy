	.global Field_YampiDesertCaveWaterOfLifeChest
Field_YampiDesertCaveWaterOfLifeChest:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_YampiDesertCaveWaterOfLifeChest, .LField_YampiDesertCaveWaterOfLifeChest_CELL_TILES
	map_block Field_YampiDesertCaveWaterOfLifeChest, .LField_YampiDesertCaveWaterOfLifeChest_SHAPES
	map_block Field_YampiDesertCaveWaterOfLifeChest, .LField_YampiDesertCaveWaterOfLifeChest_GRID
	map_block Field_YampiDesertCaveWaterOfLifeChest, .LField_YampiDesertCaveWaterOfLifeChest_SHAPE_GRID
	map_block Field_YampiDesertCaveWaterOfLifeChest, .LField_YampiDesertCaveWaterOfLifeChest_ANIMATION
	map_block Field_YampiDesertCaveWaterOfLifeChest
	map_block Field_YampiDesertCaveWaterOfLifeChest, .LField_YampiDesertCaveWaterOfLifeChest_SCENE
.LField_YampiDesertCaveWaterOfLifeChest_CELL_TILES:
	.incbin "games/tla/src/map/yampi_desert_cave_water_of_life_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_YampiDesertCaveWaterOfLifeChest_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_cave_water_of_life_chest/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertCaveWaterOfLifeChest_GRID:
	.incbin "games/tla/src/map/yampi_desert_cave_water_of_life_chest/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertCaveWaterOfLifeChest_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_cave_water_of_life_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertCaveWaterOfLifeChest_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertCaveWaterOfLifeChest_SCENE:
	.incbin "games/tla/src/map/yampi_desert_cave_water_of_life_chest/scene.scene.lz"
	.balign 4, 0
	.global Tileset_YampiDesertCaveWaterOfLifeChestPalette
Tileset_YampiDesertCaveWaterOfLifeChestPalette:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_cave_water_of_life_chest/palette.gbapal.lz"
	.byte 0x1b	@ PACKER: left in the buffer by Field_YampiDesertCave1+0x14f
	.global Tileset_YampiDesertCaveWaterOfLifeChestTilesA
Tileset_YampiDesertCaveWaterOfLifeChestTilesA:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_cave_water_of_life_chest/tiles_a.4bpp.mtf"
	.byte 0x05, 0x08	@ PACKER: unused post-stream residue; Decompress_MoveToFrontLz uses decoded RAM and output length; writer unknown
