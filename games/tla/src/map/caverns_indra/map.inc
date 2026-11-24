	.global Field_CavernsIndra
Field_CavernsIndra:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_CavernsIndra, .LField_CavernsIndra_CELL_TILES
	map_block Field_CavernsIndra, .LField_CavernsIndra_SHAPES
	map_block Field_CavernsIndra, .LField_CavernsIndra_GRID
	map_block Field_CavernsIndra, .LField_CavernsIndra_SHAPE_GRID
	map_block Field_CavernsIndra, .LField_CavernsIndra_ANIMATION
	map_block Field_CavernsIndra, .LField_CavernsIndra_BLEND
	map_block Field_CavernsIndra, .LField_CavernsIndra_SCENE
.LField_CavernsIndra_CELL_TILES:
	.incbin "games/tla/src/map/caverns_indra/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_CavernsIndra_SHAPES:
	.incbin "games/tla/src/map/caverns_indra/shapes.table.lz"
	.balign 4, 0
.LField_CavernsIndra_GRID:
	.incbin "games/tla/src/map/caverns_indra/map.grid.lz"
	.balign 4, 0
.LField_CavernsIndra_SHAPE_GRID:
	.incbin "games/tla/src/map/caverns_indra/shape_grid.plane.lz"
	.balign 4, 0
.LField_CavernsIndra_ANIMATION:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/animation.script.lz"
	.balign 4, 0
.LField_CavernsIndra_BLEND:
	.incbin "games/tla/src/map/dehkan_plateau_mint_chest/blend.script.lz"
	.balign 4, 0
.LField_CavernsIndra_SCENE:
	.incbin "games/tla/src/map/caverns_indra/scene.scene.lz"
	.global Tileset_PaletteCavernsIndra
Tileset_PaletteCavernsIndra:
	.incbin "games/tla/src/graphics/tileset/palettes/caverns_indra.gbapal.lz"
