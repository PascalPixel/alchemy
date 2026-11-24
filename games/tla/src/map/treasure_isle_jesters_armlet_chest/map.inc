	.global Field_TreasureIsleJestersArmletChest
Field_TreasureIsleJestersArmletChest:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TreasureIsleJestersArmletChest, .LField_TreasureIsleJestersArmletChest_CELL_TILES
	map_block Field_TreasureIsleJestersArmletChest, .LField_TreasureIsleJestersArmletChest_SHAPES
	map_block Field_TreasureIsleJestersArmletChest, .LField_TreasureIsleJestersArmletChest_GRID
	map_block Field_TreasureIsleJestersArmletChest, .LField_TreasureIsleJestersArmletChest_SHAPE_GRID
	map_block Field_TreasureIsleJestersArmletChest, .LField_TreasureIsleJestersArmletChest_ANIMATION
	map_block Field_TreasureIsleJestersArmletChest, .LField_TreasureIsleJestersArmletChest_BLEND
	map_block Field_TreasureIsleJestersArmletChest, .LField_TreasureIsleJestersArmletChest_SCENE
.LField_TreasureIsleJestersArmletChest_CELL_TILES:
	.incbin "games/tla/src/map/treasure_isle_jesters_armlet_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_TreasureIsleJestersArmletChest_SHAPES:
	.incbin "games/tla/src/map/treasure_isle_jesters_armlet_chest/shapes.table.lz"
	.balign 4, 0
.LField_TreasureIsleJestersArmletChest_GRID:
	.incbin "games/tla/src/map/treasure_isle_jesters_armlet_chest/map.grid.lz"
	.balign 4, 0
.LField_TreasureIsleJestersArmletChest_SHAPE_GRID:
	.incbin "games/tla/src/map/treasure_isle_jesters_armlet_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_TreasureIsleJestersArmletChest_ANIMATION:
	.incbin "games/tla/src/map/treasure_isle_chests/animation.script.lz"
	.balign 4, 0
.LField_TreasureIsleJestersArmletChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TreasureIsleJestersArmletChest_SCENE:
	.incbin "games/tla/src/map/treasure_isle_jesters_armlet_chest/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TreasureIsleJestersArmletChestPalette
Tileset_TreasureIsleJestersArmletChestPalette:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_jesters_armlet_chest/palette.gbapal.lz"
	.byte 0x83, 0xf0	@ PACKER: left in the buffer by Field_TreasureIsle3+0x152
	.global Tileset_TreasureIsleJestersArmletChestTilesA
Tileset_TreasureIsleJestersArmletChestTilesA:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_jesters_armlet_chest/tiles_a.4bpp.mtf"
	.global Tileset_TreasureIsleJestersArmletChestTilesB
Tileset_TreasureIsleJestersArmletChestTilesB:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_jesters_armlet_chest/tiles_b.4bpp.mtf"
	.byte 0x0f	@ PACKER: left in the buffer by Tileset_TreasureIsleJestersArmletChestTilesA+0x172b
	.global Tileset_TreasureIsleJestersArmletChestTilesC
Tileset_TreasureIsleJestersArmletChestTilesC:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_jesters_armlet_chest/tiles_c.4bpp.mtf"
	.global Tileset_TreasureIsleJestersArmletChestTilesD
Tileset_TreasureIsleJestersArmletChestTilesD:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_jesters_armlet_chest/tiles_d.4bpp.mtf"
	.byte 0x73	@ PACKER: left in the buffer by Tileset_TreasureIsleJestersArmletChestTilesC+0x4e3
