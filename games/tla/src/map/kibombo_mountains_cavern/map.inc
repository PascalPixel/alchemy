	.global Field_KibomboMountainsCavern
Field_KibomboMountainsCavern:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KibomboMountainsCavern, .LField_KibomboMountainsCavern_CELL_TILES
	map_block Field_KibomboMountainsCavern, .LField_KibomboMountainsCavern_SHAPES
	map_block Field_KibomboMountainsCavern, .LField_KibomboMountainsCavern_GRID
	map_block Field_KibomboMountainsCavern, .LField_KibomboMountainsCavern_SHAPE_GRID
	map_block Field_KibomboMountainsCavern, .LField_KibomboMountainsCavern_ANIMATION
	map_block Field_KibomboMountainsCavern, .LField_KibomboMountainsCavern_BLEND
	map_block Field_KibomboMountainsCavern, .LField_KibomboMountainsCavern_SCENE
.LField_KibomboMountainsCavern_CELL_TILES:
	.incbin "games/tla/src/map/kibombo_mountains_cavern/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_KibomboMountainsCavern_SHAPES:
	.incbin "games/tla/src/map/kibombo_mountains_cavern/shapes.table.lz"
	.balign 4, 0
.LField_KibomboMountainsCavern_GRID:
	.incbin "games/tla/src/map/kibombo_mountains_cavern/map.grid.lz"
	.balign 4, 0
.LField_KibomboMountainsCavern_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_mountains_cavern/shape_grid.plane.lz"
	.balign 4, 0
.LField_KibomboMountainsCavern_ANIMATION:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/animation.script.lz"
	.balign 4, 0
.LField_KibomboMountainsCavern_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KibomboMountainsCavern_SCENE:
	.incbin "games/tla/src/map/kibombo_mountains_cavern/scene.scene.lz"
	.balign 4, 0
	.global Tileset_KibomboMountainsCavernPalette
Tileset_KibomboMountainsCavernPalette:
	.incbin "games/tla/src/graphics/tileset/kibombo_mountains_cavern/palette.gbapal.lz"
	.global Tileset_KibomboMountainsCavernTilesA
Tileset_KibomboMountainsCavernTilesA:
	.incbin "games/tla/src/graphics/tileset/kibombo_mountains_cavern/tiles_a.4bpp.mtf"
	.byte 0xf6, 0x09, 0x04	@ PACKER: left in the buffer by Field_KibomboMountains2+0x1985
	.global Tileset_KibomboMountainsCavernTilesB
Tileset_KibomboMountainsCavernTilesB:
	.incbin "games/tla/src/graphics/tileset/kibombo_mountains_cavern/tiles_b.4bpp.mtf"
	.byte 0x1d, 0x94, 0x7f	@ PACKER: left in the buffer by Tileset_KibomboMountainsCavernTilesA+0x148d
	.global Tileset_KibomboMountainsCavernTilesC
Tileset_KibomboMountainsCavernTilesC:
	.incbin "games/tla/src/graphics/tileset/kibombo_mountains_cavern/tiles_c.4bpp.mtf"
	.byte 0xe4	@ PACKER: left in the buffer by Tileset_KibomboMountainsCavernTilesB+0x467
	.global Tileset_KibomboMountainsCavernTilesD
Tileset_KibomboMountainsCavernTilesD:
	.incbin "games/tla/src/graphics/tileset/kibombo_mountains_cavern/tiles_d.4bpp.mtf"
	.byte 0x7f, 0xe0	@ PACKER: left in the buffer by Tileset_KibomboMountainsCavernTilesC+0x1be
