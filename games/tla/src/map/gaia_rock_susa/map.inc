	.global Field_GaiaRockSusa
Field_GaiaRockSusa:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GaiaRockSusa, .LField_GaiaRockSusa_CELL_TILES
	map_block Field_GaiaRockSusa, .LField_GaiaRockSusa_SHAPES
	map_block Field_GaiaRockSusa, .LField_GaiaRockSusa_GRID
	map_block Field_GaiaRockSusa, .LField_GaiaRockSusa_SHAPE_GRID
	map_block Field_GaiaRockSusa, .LField_GaiaRockSusa_ANIMATION
	map_block Field_GaiaRockSusa, .LField_GaiaRockSusa_BLEND
	map_block Field_GaiaRockSusa, .LField_GaiaRockSusa_SCENE
.LField_GaiaRockSusa_CELL_TILES:
	.incbin "games/tla/src/map/gaia_rock_susa/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GaiaRockSusa_SHAPES:
	.incbin "games/tla/src/map/gaia_rock_susa/shapes.table.lz"
	.balign 4, 0
.LField_GaiaRockSusa_GRID:
	.incbin "games/tla/src/map/gaia_rock_susa/map.grid.lz"
	.balign 4, 0
.LField_GaiaRockSusa_SHAPE_GRID:
	.incbin "games/tla/src/map/gaia_rock_susa/shape_grid.plane.lz"
	.balign 4, 0
.LField_GaiaRockSusa_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GaiaRockSusa_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GaiaRockSusa_SCENE:
	.incbin "games/tla/src/map/gaia_rock_susa/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGaiaRockSusa
Tileset_PaletteGaiaRockSusa:
	.incbin "games/tla/src/graphics/tileset/palettes/gaia_rock_susa.gbapal.lz"
	.byte 0x1a	@ PACKER: left in the buffer by Field_GaiaRock1+0x187
