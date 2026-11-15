	.global Field_GaiaRock1
Field_GaiaRock1:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GaiaRock1, .LField_GaiaRock1_CELL_PICTURES
	map_block Field_GaiaRock1, .LField_GaiaRock1_SHAPES
	map_block Field_GaiaRock1, .LField_GaiaRock1_GRID
	map_block Field_GaiaRock1, .LField_GaiaRock1_SHAPE_GRID
	map_block Field_GaiaRock1, .LField_GaiaRock1_ANIMATION
	map_block Field_GaiaRock1, .LField_GaiaRock1_BLEND
	map_block Field_GaiaRock1, .LField_GaiaRock1_SCENE
.LField_GaiaRock1_CELL_PICTURES:
	.incbin "games/tla/src/map/gaia_rock_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GaiaRock1_SHAPES:
	.incbin "games/tla/src/map/gaia_rock_1/shapes.table.lz"
	.balign 4, 0
.LField_GaiaRock1_GRID:
	.incbin "games/tla/src/map/gaia_rock_1/map.grid.lz"
	.balign 4, 0
.LField_GaiaRock1_SHAPE_GRID:
	.incbin "games/tla/src/map/gaia_rock_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_GaiaRock1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_GaiaRock1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GaiaRock1_SCENE:
	.incbin "games/tla/src/map/gaia_rock_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGaiaRock1
Tileset_PaletteGaiaRock1:
	.incbin "games/tla/src/graphics/tileset/palettes/gaia_rock_1.gbapal.lz"
	.byte 0x1a	@ PACKER: left in the buffer by Field_GaiaRock1+0x187
