	.global Field_DehkanPlateauMintChest
Field_DehkanPlateauMintChest:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DehkanPlateauMintChest, .LField_DehkanPlateauMintChest_CELL_TILES
	map_block Field_DehkanPlateauMintChest, .LField_DehkanPlateauMintChest_SHAPES
	map_block Field_DehkanPlateauMintChest, .LField_DehkanPlateauMintChest_GRID
	map_block Field_DehkanPlateauMintChest, .LField_DehkanPlateauMintChest_SHAPE_GRID
	map_block Field_DehkanPlateauMintChest, .LField_DehkanPlateauMintChest_ANIMATION
	map_block Field_DehkanPlateauMintChest, .LField_DehkanPlateauMintChest_BLEND
	map_block Field_DehkanPlateauMintChest, .LField_DehkanPlateauMintChest_SCENE
.LField_DehkanPlateauMintChest_CELL_TILES:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_DehkanPlateauMintChest_SHAPES:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/shapes.table.lz"
	.balign 4, 0
.LField_DehkanPlateauMintChest_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/map.grid.lz"
	.balign 4, 0
.LField_DehkanPlateauMintChest_SHAPE_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_DehkanPlateauMintChest_ANIMATION:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/animation.script.lz"
	.balign 4, 0
.LField_DehkanPlateauMintChest_BLEND:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/blend.script.lz"
	.balign 4, 0
.LField_DehkanPlateauMintChest_SCENE:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DehkanPlateauMintChestPalette
Tileset_DehkanPlateauMintChestPalette:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_mint_chest/palette.gbapal.lz"
	.byte 0x99, 0xac	@ PACKER: left in the buffer by Field_DehkanPlateauMintChest+0x10a
	.global Tileset_DehkanPlateauMintChestTilesA
Tileset_DehkanPlateauMintChestTilesA:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_mint_chest/tiles_a.4bpp.mtf"
	.byte 0xf8, 0x3e	@ PACKER: left in the buffer by Tileset_DehkanPlateauTightropesTilesC+0x1b56
	.global Tileset_DehkanPlateauMintChestTilesB
Tileset_DehkanPlateauMintChestTilesB:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_mint_chest/tiles_b.4bpp.mtf"
	.global Tileset_DehkanPlateauMintChestTilesC
Tileset_DehkanPlateauMintChestTilesC:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_mint_chest/tiles_c.4bpp.mtf"
	.byte 0x83, 0xb4, 0x9c	@ PACKER: left in the buffer by Tileset_DehkanPlateauMintChestTilesB+0x1f5
	.global Tileset_DehkanPlateauMintChestTilesD
Tileset_DehkanPlateauMintChestTilesD:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_mint_chest/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_DehkanPlateauMintChestTilesC+0x1be
