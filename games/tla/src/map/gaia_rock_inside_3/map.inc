	.global Field_GaiaRockInside3
Field_GaiaRockInside3:
	map_header 0, 0, 130, 112, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GaiaRockInside3, .LField_GaiaRockInside3_CELL_PICTURES
	map_block Field_GaiaRockInside3, .LField_GaiaRockInside3_SHAPES
	map_block Field_GaiaRockInside3, .LField_GaiaRockInside3_GRID
	map_block Field_GaiaRockInside3, .LField_GaiaRockInside3_SHAPE_GRID
	map_block Field_GaiaRockInside3, .LField_GaiaRockInside3_ANIMATION
	map_block Field_GaiaRockInside3, .LField_GaiaRockInside3_BLEND
	map_block Field_GaiaRockInside3, .LField_GaiaRockInside3_SCENE
.LField_GaiaRockInside3_CELL_PICTURES:
	.incbin "games/tla/src/map/gaia_rock_inside_3/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GaiaRockInside3_SHAPES:
	.incbin "games/tla/src/map/gaia_rock_inside_3/shapes.table.lz"
	.balign 4, 0
.LField_GaiaRockInside3_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_3/map.grid.lz"
	.balign 4, 0
.LField_GaiaRockInside3_SHAPE_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_GaiaRockInside3_ANIMATION:
	.incbin "games/tla/src/map/gaia_rock_inside_3/animation.script.lz"
	.balign 4, 0
.LField_GaiaRockInside3_BLEND:
	.incbin "games/tla/src/map/gaia_rock_inside_3/blend.script.lz"
	.balign 4, 0
.LField_GaiaRockInside3_SCENE:
	.incbin "games/tla/src/map/gaia_rock_inside_3/scene.scene.lz"
	.balign 4, 0
