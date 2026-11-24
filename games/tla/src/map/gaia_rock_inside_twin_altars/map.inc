	.global Field_GaiaRockInsideTwinAltars
Field_GaiaRockInsideTwinAltars:
	map_header 0, 0, 130, 112, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GaiaRockInsideTwinAltars, .LField_GaiaRockInsideTwinAltars_CELL_TILES
	map_block Field_GaiaRockInsideTwinAltars, .LField_GaiaRockInsideTwinAltars_SHAPES
	map_block Field_GaiaRockInsideTwinAltars, .LField_GaiaRockInsideTwinAltars_GRID
	map_block Field_GaiaRockInsideTwinAltars, .LField_GaiaRockInsideTwinAltars_SHAPE_GRID
	map_block Field_GaiaRockInsideTwinAltars, .LField_GaiaRockInsideTwinAltars_ANIMATION
	map_block Field_GaiaRockInsideTwinAltars, .LField_GaiaRockInsideTwinAltars_BLEND
	map_block Field_GaiaRockInsideTwinAltars, .LField_GaiaRockInsideTwinAltars_SCENE
.LField_GaiaRockInsideTwinAltars_CELL_TILES:
	.incbin "games/tla/src/map/gaia_rock_inside_twin_altars/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GaiaRockInsideTwinAltars_SHAPES:
	.incbin "games/tla/src/map/gaia_rock_inside_twin_altars/shapes.table.lz"
	.balign 4, 0
.LField_GaiaRockInsideTwinAltars_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_twin_altars/map.grid.lz"
	.balign 4, 0
.LField_GaiaRockInsideTwinAltars_SHAPE_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_twin_altars/shape_grid.plane.lz"
	.balign 4, 0
.LField_GaiaRockInsideTwinAltars_ANIMATION:
	.incbin "games/tla/src/map/gaia_rock_inside_twin_altars/animation.script.lz"
	.balign 4, 0
.LField_GaiaRockInsideTwinAltars_BLEND:
	.incbin "games/tla/src/map/gaia_rock_inside_twin_altars/blend.script.lz"
	.balign 4, 0
.LField_GaiaRockInsideTwinAltars_SCENE:
	.incbin "games/tla/src/map/gaia_rock_inside_twin_altars/scene.scene.lz"
	.balign 4, 0
