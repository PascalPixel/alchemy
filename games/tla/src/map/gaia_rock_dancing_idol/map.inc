	.global Field_GaiaRockDancingIdol
Field_GaiaRockDancingIdol:
	map_header 0, 0, 128, 128, 2, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 164, 4, 4, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GaiaRockDancingIdol, .LField_GaiaRockDancingIdol_CELL_TILES
	map_block Field_GaiaRockDancingIdol, .LField_GaiaRockDancingIdol_SHAPES
	map_block Field_GaiaRockDancingIdol, .LField_GaiaRockDancingIdol_GRID
	map_block Field_GaiaRockDancingIdol, .LField_GaiaRockDancingIdol_SHAPE_GRID
	map_block Field_GaiaRockDancingIdol, .LField_GaiaRockDancingIdol_ANIMATION
	map_block Field_GaiaRockDancingIdol, .LField_GaiaRockDancingIdol_BLEND
	map_block Field_GaiaRockDancingIdol, .LField_GaiaRockDancingIdol_SCENE
.LField_GaiaRockDancingIdol_CELL_TILES:
	.incbin "games/tla/src/map/gaia_rock_dancing_idol/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GaiaRockDancingIdol_SHAPES:
	.incbin "games/tla/src/map/gaia_rock_dancing_idol/shapes.table.lz"
	.balign 4, 0
.LField_GaiaRockDancingIdol_GRID:
	.incbin "games/tla/src/map/gaia_rock_dancing_idol/map.grid.lz"
	.balign 4, 0
.LField_GaiaRockDancingIdol_SHAPE_GRID:
	.incbin "games/tla/src/map/gaia_rock_dancing_idol/shape_grid.plane.lz"
	.balign 4, 0
.LField_GaiaRockDancingIdol_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GaiaRockDancingIdol_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GaiaRockDancingIdol_SCENE:
	.incbin "games/tla/src/map/gaia_rock_dancing_idol/scene.scene.lz"
	.balign 4, 0
