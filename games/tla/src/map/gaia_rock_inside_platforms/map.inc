	.global Field_GaiaRockInsidePlatforms
Field_GaiaRockInsidePlatforms:
	map_header 0, 0, 130, 112, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GaiaRockInsidePlatforms, .LField_GaiaRockInsidePlatforms_CELL_TILES
	map_block Field_GaiaRockInsidePlatforms, .LField_GaiaRockInsidePlatforms_SHAPES
	map_block Field_GaiaRockInsidePlatforms, .LField_GaiaRockInsidePlatforms_GRID
	map_block Field_GaiaRockInsidePlatforms, .LField_GaiaRockInsidePlatforms_SHAPE_GRID
	map_block Field_GaiaRockInsidePlatforms, .LField_GaiaRockInsidePlatforms_ANIMATION
	map_block Field_GaiaRockInsidePlatforms, .LField_GaiaRockInsidePlatforms_BLEND
	map_block Field_GaiaRockInsidePlatforms, .LField_GaiaRockInsidePlatforms_SCENE
.LField_GaiaRockInsidePlatforms_CELL_TILES:
	.incbin "games/tla/src/map/gaia_rock_inside_platforms/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_GaiaRockInsidePlatforms_SHAPES:
	.incbin "games/tla/src/map/gaia_rock_inside_platforms/shapes.table.lz"
	.balign 4, 0
.LField_GaiaRockInsidePlatforms_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_platforms/map.grid.lz"
	.balign 4, 0
.LField_GaiaRockInsidePlatforms_SHAPE_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_platforms/shape_grid.plane.lz"
	.balign 4, 0
.LField_GaiaRockInsidePlatforms_ANIMATION:
	.incbin "games/tla/src/map/gaia_rock_inside_platforms/animation.script.lz"
	.balign 4, 0
.LField_GaiaRockInsidePlatforms_BLEND:
	.incbin "games/tla/src/map/gaia_rock_inside_first_altar/blend.script.lz"
	.balign 4, 0
.LField_GaiaRockInsidePlatforms_SCENE:
	.incbin "games/tla/src/map/gaia_rock_inside_platforms/scene.scene.lz"
