	.global Field_DehkanPlateauCannon
Field_DehkanPlateauCannon:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DehkanPlateauCannon, .LField_DehkanPlateauCannon_CELL_TILES
	map_block Field_DehkanPlateauCannon, .LField_DehkanPlateauCannon_SHAPES
	map_block Field_DehkanPlateauCannon, .LField_DehkanPlateauCannon_GRID
	map_block Field_DehkanPlateauCannon, .LField_DehkanPlateauCannon_SHAPE_GRID
	map_block Field_DehkanPlateauCannon, .LField_DehkanPlateauCannon_ANIMATION
	map_block Field_DehkanPlateauCannon, .LField_DehkanPlateauCannon_BLEND
	map_block Field_DehkanPlateauCannon, .LField_DehkanPlateauCannon_SCENE
.LField_DehkanPlateauCannon_CELL_TILES:
	.incbin "games/tla/src/map/dehkan_plateau_cannon/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_DehkanPlateauCannon_SHAPES:
	.incbin "games/tla/src/map/dehkan_plateau_cannon/shapes.table.lz"
	.balign 4, 0
.LField_DehkanPlateauCannon_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_cannon/map.grid.lz"
	.balign 4, 0
.LField_DehkanPlateauCannon_SHAPE_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_cannon/shape_grid.plane.lz"
	.balign 4, 0
.LField_DehkanPlateauCannon_ANIMATION:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/animation.script.lz"
	.balign 4, 0
.LField_DehkanPlateauCannon_BLEND:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/blend.script.lz"
	.balign 4, 0
.LField_DehkanPlateauCannon_SCENE:
	.incbin "games/tla/src/map/dehkan_plateau_cannon/scene.scene.lz"
	.global Tileset_DehkanPlateauCannonPalette
Tileset_DehkanPlateauCannonPalette:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_cannon/palette.gbapal.lz"
	.global Tileset_DehkanPlateauCannonTilesA
Tileset_DehkanPlateauCannonTilesA:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_cannon/tiles_a.4bpp.mtf"
	.global Tileset_DehkanPlateauCannonTilesB
Tileset_DehkanPlateauCannonTilesB:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_cannon/tiles_b.4bpp.mtf"
	.global Tileset_DehkanPlateauCannonTilesC
Tileset_DehkanPlateauCannonTilesC:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_cannon/tiles_c.4bpp.mtf"
	.byte 0x83, 0xb4, 0x9c	@ PACKER: left in the buffer by Tileset_DehkanPlateauCannonTilesB+0x1f5
	.global Tileset_DehkanPlateauCannonTilesD
Tileset_DehkanPlateauCannonTilesD:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_cannon/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_DehkanPlateauCannonTilesC+0x1be
