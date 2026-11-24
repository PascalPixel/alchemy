	.global Field_DehkanPlateauThemisAxeChest
Field_DehkanPlateauThemisAxeChest:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DehkanPlateauThemisAxeChest, .LField_DehkanPlateauThemisAxeChest_CELL_TILES
	map_block Field_DehkanPlateauThemisAxeChest, .LField_DehkanPlateauThemisAxeChest_SHAPES
	map_block Field_DehkanPlateauThemisAxeChest, .LField_DehkanPlateauThemisAxeChest_GRID
	map_block Field_DehkanPlateauThemisAxeChest, .LField_DehkanPlateauThemisAxeChest_SHAPE_GRID
	map_block Field_DehkanPlateauThemisAxeChest, .LField_DehkanPlateauThemisAxeChest_ANIMATION
	map_block Field_DehkanPlateauThemisAxeChest, .LField_DehkanPlateauThemisAxeChest_BLEND
	map_block Field_DehkanPlateauThemisAxeChest, .LField_DehkanPlateauThemisAxeChest_SCENE
.LField_DehkanPlateauThemisAxeChest_CELL_TILES:
	.incbin "games/tla/src/map/dehkan_plateau_themis_axe_chest/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_DehkanPlateauThemisAxeChest_SHAPES:
	.incbin "games/tla/src/map/dehkan_plateau_themis_axe_chest/shapes.table.lz"
	.balign 4, 0
.LField_DehkanPlateauThemisAxeChest_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_themis_axe_chest/map.grid.lz"
	.balign 4, 0
.LField_DehkanPlateauThemisAxeChest_SHAPE_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_themis_axe_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_DehkanPlateauThemisAxeChest_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_DehkanPlateauThemisAxeChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_DehkanPlateauThemisAxeChest_SCENE:
	.incbin "games/tla/src/map/dehkan_plateau_themis_axe_chest/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DehkanPlateauThemisAxeChestPalette
Tileset_DehkanPlateauThemisAxeChestPalette:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_themis_axe_chest/palette.gbapal.lz"
	.byte 0x98	@ PACKER: left in the buffer by Field_DehkanPlateau2+0x14f
	.global Tileset_DehkanPlateauThemisAxeChestTilesA
Tileset_DehkanPlateauThemisAxeChestTilesA:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_themis_axe_chest/tiles_a.4bpp.mtf"
	.global Tileset_DehkanPlateauThemisAxeChestTilesB
Tileset_DehkanPlateauThemisAxeChestTilesB:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_themis_axe_chest/tiles_b.4bpp.mtf"
	.byte 0xe7, 0x01	@ PACKER: left in the buffer by Tileset_DehkanPlateauThemisAxeChestTilesA+0xfda
	.global Tileset_DehkanPlateauThemisAxeChestTilesC
Tileset_DehkanPlateauThemisAxeChestTilesC:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_themis_axe_chest/tiles_c.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Field_DehkanPlateau2+0x217f
	.global Tileset_DehkanPlateauThemisAxeChestTilesD
Tileset_DehkanPlateauThemisAxeChestTilesD:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_themis_axe_chest/tiles_d.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Tileset_DehkanPlateauThemisAxeChestTilesC+0x13f
