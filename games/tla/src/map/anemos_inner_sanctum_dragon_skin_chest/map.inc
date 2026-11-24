	.global Field_AnemosInnerSanctumDragonSkinChest
Field_AnemosInnerSanctumDragonSkinChest:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnemosInnerSanctumDragonSkinChest, .LField_AnemosInnerSanctumDragonSkinChest_CELL_TILES
	map_block Field_AnemosInnerSanctumDragonSkinChest, .LField_AnemosInnerSanctumDragonSkinChest_SHAPES
	map_block Field_AnemosInnerSanctumDragonSkinChest, .LField_AnemosInnerSanctumDragonSkinChest_GRID
	map_block Field_AnemosInnerSanctumDragonSkinChest, .LField_AnemosInnerSanctumDragonSkinChest_SHAPE_GRID
	map_block Field_AnemosInnerSanctumDragonSkinChest, .LField_AnemosInnerSanctumDragonSkinChest_ANIMATION
	map_block Field_AnemosInnerSanctumDragonSkinChest, .LField_AnemosInnerSanctumDragonSkinChest_BLEND
	map_block Field_AnemosInnerSanctumDragonSkinChest, .LField_AnemosInnerSanctumDragonSkinChest_SCENE
.LField_AnemosInnerSanctumDragonSkinChest_CELL_TILES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_dragon_skin_chest/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumDragonSkinChest_SHAPES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_dragon_skin_chest/shapes.table.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumDragonSkinChest_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_dragon_skin_chest/map.grid.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumDragonSkinChest_SHAPE_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_dragon_skin_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumDragonSkinChest_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumDragonSkinChest_BLEND:
	.incbin "games/tla/src/map/anemos_inner_sanctum_dragon_skin_chest/blend.script.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumDragonSkinChest_SCENE:
	.incbin "games/tla/src/map/anemos_inner_sanctum_dragon_skin_chest/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AnemosInnerSanctumDragonSkinChestPalette
Tileset_AnemosInnerSanctumDragonSkinChestPalette:
	.incbin "games/tla/src/graphics/tileset/anemos_inner_sanctum_dragon_skin_chest/palette.gbapal.lz"
	.byte 0x06	@ PACKER: left in the buffer by Field_AnemosInnerSanctum1+0x193
	.global Tileset_AnemosInnerSanctumDragonSkinChestTilesA
Tileset_AnemosInnerSanctumDragonSkinChestTilesA:
	.incbin "games/tla/src/graphics/tileset/anemos_inner_sanctum_dragon_skin_chest/tiles_a.4bpp.mtf"
	.byte 0x72	@ PACKER: left in the buffer by Field_ShamanVillage4+0x20b3
