	.global Field_GaiaRockStatueSpots
Field_GaiaRockStatueSpots:
	map_header 0, 0, 128, 128, 2, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 164, 4, 4, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GaiaRockStatueSpots, .LField_GaiaRockStatueSpots_CELL_TILES
	map_block Field_GaiaRockStatueSpots, .LField_GaiaRockStatueSpots_SHAPES
	map_block Field_GaiaRockStatueSpots, .LField_GaiaRockStatueSpots_GRID
	map_block Field_GaiaRockStatueSpots, .LField_GaiaRockStatueSpots_SHAPE_GRID
	map_block Field_GaiaRockStatueSpots, .LField_GaiaRockStatueSpots_ANIMATION
	map_block Field_GaiaRockStatueSpots, .LField_GaiaRockStatueSpots_BLEND
	map_block Field_GaiaRockStatueSpots, .LField_GaiaRockStatueSpots_SCENE
.LField_GaiaRockStatueSpots_CELL_TILES:
	.incbin "games/tla/src/map/gaia_rock_statue_spots/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GaiaRockStatueSpots_SHAPES:
	.incbin "games/tla/src/map/gaia_rock_statue_spots/shapes.table.lz"
	.balign 4, 0
.LField_GaiaRockStatueSpots_GRID:
	.incbin "games/tla/src/map/gaia_rock_statue_spots/map.grid.lz"
	.balign 4, 0
.LField_GaiaRockStatueSpots_SHAPE_GRID:
	.incbin "games/tla/src/map/gaia_rock_statue_spots/shape_grid.plane.lz"
	.balign 4, 0
.LField_GaiaRockStatueSpots_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GaiaRockStatueSpots_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GaiaRockStatueSpots_SCENE:
	.incbin "games/tla/src/map/gaia_rock_statue_spots/scene.scene.lz"
	.balign 4, 0
