	.global Field_DehkanPlateauFullMetalVestChest
Field_DehkanPlateauFullMetalVestChest:
	map_header 0, 0, 128, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DehkanPlateauFullMetalVestChest, .LField_DehkanPlateauFullMetalVestChest_CELL_TILES
	map_block Field_DehkanPlateauFullMetalVestChest, .LField_DehkanPlateauFullMetalVestChest_SHAPES
	map_block Field_DehkanPlateauFullMetalVestChest, .LField_DehkanPlateauFullMetalVestChest_GRID
	map_block Field_DehkanPlateauFullMetalVestChest, .LField_DehkanPlateauFullMetalVestChest_SHAPE_GRID
	map_block Field_DehkanPlateauFullMetalVestChest, .LField_DehkanPlateauFullMetalVestChest_ANIMATION
	map_block Field_DehkanPlateauFullMetalVestChest, .LField_DehkanPlateauFullMetalVestChest_BLEND
	map_block Field_DehkanPlateauFullMetalVestChest, .LField_DehkanPlateauFullMetalVestChest_SCENE
.LField_DehkanPlateauFullMetalVestChest_CELL_TILES:
	.incbin "games/tla/src/map/dehkan_plateau_full_metal_vest_chest/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_DehkanPlateauFullMetalVestChest_SHAPES:
	.incbin "games/tla/src/map/dehkan_plateau_full_metal_vest_chest/shapes.table.lz"
	.balign 4, 0
.LField_DehkanPlateauFullMetalVestChest_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/dehkan_plateau_full_metal_vest_chest/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/dehkan_plateau_full_metal_vest_chest/map.grid.lz"
	.endif
	.balign 4, 0
.LField_DehkanPlateauFullMetalVestChest_SHAPE_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_full_metal_vest_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_DehkanPlateauFullMetalVestChest_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_DehkanPlateauFullMetalVestChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_DehkanPlateauFullMetalVestChest_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/dehkan_plateau_full_metal_vest_chest/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/dehkan_plateau_full_metal_vest_chest/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_DehkanPlateauFullMetalVestChestPalette
Tileset_DehkanPlateauFullMetalVestChestPalette:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_full_metal_vest_chest/palette.gbapal.lz"
	.byte 0xc2, 0x8d	@ PACKER: left in the buffer by Field_DehkanPlateau1+0x14e
	.global Tileset_DehkanPlateauFullMetalVestChestTilesA
Tileset_DehkanPlateauFullMetalVestChestTilesA:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_full_metal_vest_chest/tiles_a.4bpp.mtf"
	.global Tileset_DehkanPlateauFullMetalVestChestTilesB
Tileset_DehkanPlateauFullMetalVestChestTilesB:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_full_metal_vest_chest/tiles_b.4bpp.mtf"
	.byte 0xe7, 0x01	@ PACKER: left in the buffer by Tileset_DehkanPlateauFullMetalVestChestTilesA+0xfda
	.global Tileset_DehkanPlateauFullMetalVestChestTilesC
Tileset_DehkanPlateauFullMetalVestChestTilesC:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_full_metal_vest_chest/tiles_c.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Field_DehkanPlateau1+0x217f
	.global Tileset_DehkanPlateauFullMetalVestChestTilesD
Tileset_DehkanPlateauFullMetalVestChestTilesD:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_full_metal_vest_chest/tiles_d.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Tileset_DehkanPlateauFullMetalVestChestTilesC+0x13f
