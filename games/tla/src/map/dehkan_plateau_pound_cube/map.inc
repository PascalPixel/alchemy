	.global Field_DehkanPlateauPoundCube
Field_DehkanPlateauPoundCube:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DehkanPlateauPoundCube, .LField_DehkanPlateauPoundCube_CELL_TILES
	map_block Field_DehkanPlateauPoundCube, .LField_DehkanPlateauPoundCube_SHAPES
	map_block Field_DehkanPlateauPoundCube, .LField_DehkanPlateauPoundCube_GRID
	map_block Field_DehkanPlateauPoundCube, .LField_DehkanPlateauPoundCube_SHAPE_GRID
	map_block Field_DehkanPlateauPoundCube, .LField_DehkanPlateauPoundCube_ANIMATION
	map_block Field_DehkanPlateauPoundCube, .LField_DehkanPlateauPoundCube_BLEND
	map_block Field_DehkanPlateauPoundCube, .LField_DehkanPlateauPoundCube_SCENE
.LField_DehkanPlateauPoundCube_CELL_TILES:
	.incbin "games/tla/src/map/dehkan_plateau_pound_cube/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_DehkanPlateauPoundCube_SHAPES:
	.incbin "games/tla/src/map/dehkan_plateau_pound_cube/shapes.table.lz"
	.balign 4, 0
.LField_DehkanPlateauPoundCube_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_pound_cube/map.grid.lz"
	.balign 4, 0
.LField_DehkanPlateauPoundCube_SHAPE_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_pound_cube/shape_grid.plane.lz"
	.balign 4, 0
.LField_DehkanPlateauPoundCube_ANIMATION:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/animation.script.lz"
	.balign 4, 0
.LField_DehkanPlateauPoundCube_BLEND:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/blend.script.lz"
	.balign 4, 0
.LField_DehkanPlateauPoundCube_SCENE:
	.incbin "games/tla/src/map/dehkan_plateau_pound_cube/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DehkanPlateauPoundCubePalette
Tileset_DehkanPlateauPoundCubePalette:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_pound_cube/palette.gbapal.lz"
	.global Tileset_DehkanPlateauPoundCubeTilesA
Tileset_DehkanPlateauPoundCubeTilesA:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_pound_cube/tiles_a.4bpp.mtf"
	.byte 0xf8, 0x3e	@ PACKER: left in the buffer by Tileset_DehkanPlateauMintChestTilesA+0x1b56
	.global Tileset_DehkanPlateauPoundCubeTilesB
Tileset_DehkanPlateauPoundCubeTilesB:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_pound_cube/tiles_b.4bpp.mtf"
	.global Tileset_DehkanPlateauPoundCubeTilesC
Tileset_DehkanPlateauPoundCubeTilesC:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_pound_cube/tiles_c.4bpp.mtf"
	.byte 0x83, 0xb4, 0x9c	@ PACKER: left in the buffer by Tileset_DehkanPlateauPoundCubeTilesB+0x1f5
	.global Tileset_DehkanPlateauPoundCubeTilesD
Tileset_DehkanPlateauPoundCubeTilesD:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_pound_cube/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_DehkanPlateauPoundCubeTilesC+0x1be
