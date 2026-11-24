	.global Field_TreasureIsleChests
Field_TreasureIsleChests:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TreasureIsleChests, .LField_TreasureIsleChests_CELL_TILES
	map_block Field_TreasureIsleChests, .LField_TreasureIsleChests_SHAPES
	map_block Field_TreasureIsleChests, .LField_TreasureIsleChests_GRID
	map_block Field_TreasureIsleChests, .LField_TreasureIsleChests_SHAPE_GRID
	map_block Field_TreasureIsleChests, .LField_TreasureIsleChests_ANIMATION
	map_block Field_TreasureIsleChests, .LField_TreasureIsleChests_BLEND
	map_block Field_TreasureIsleChests, .LField_TreasureIsleChests_SCENE
.LField_TreasureIsleChests_CELL_TILES:
	.incbin "games/tla/src/map/treasure_isle_chests/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_TreasureIsleChests_SHAPES:
	.incbin "games/tla/src/map/treasure_isle_chests/shapes.table.lz"
	.balign 4, 0
.LField_TreasureIsleChests_GRID:
	.incbin "games/tla/src/map/treasure_isle_chests/map.grid.lz"
	.balign 4, 0
.LField_TreasureIsleChests_SHAPE_GRID:
	.incbin "games/tla/src/map/treasure_isle_chests/shape_grid.plane.lz"
	.balign 4, 0
.LField_TreasureIsleChests_ANIMATION:
	.incbin "games/tla/src/map/treasure_isle_chests/animation.script.lz"
	.balign 4, 0
.LField_TreasureIsleChests_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TreasureIsleChests_SCENE:
	.incbin "games/tla/src/map/treasure_isle_chests/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TreasureIsleChestsPalette
Tileset_TreasureIsleChestsPalette:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_chests/palette.gbapal.lz"
	.byte 0x17, 0x3c	@ PACKER: left in the buffer by Field_TreasureIsleChests+0x152
	.global Tileset_TreasureIsleChestsTilesA
Tileset_TreasureIsleChestsTilesA:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_chests/tiles_a.4bpp.mtf"
	.global Tileset_TreasureIsleChestsTilesB
Tileset_TreasureIsleChestsTilesB:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_chests/tiles_b.4bpp.mtf"
	.byte 0x97	@ PACKER: left in the buffer by Tileset_TreasureIsleChestsTilesA+0x166b
	.global Tileset_TreasureIsleChestsTilesC
Tileset_TreasureIsleChestsTilesC:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_chests/tiles_c.4bpp.mtf"
	.global Tileset_TreasureIsleChestsTilesD
Tileset_TreasureIsleChestsTilesD:
	.incbin "games/tla/src/graphics/tileset/treasure_isle_chests/tiles_d.4bpp.mtf"
	.byte 0x16, 0x41	@ PACKER: left in the buffer by Tileset_TreasureIsleChestsTilesC+0x776
