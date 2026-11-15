	.global Field_GaiaRockInside4
Field_GaiaRockInside4:
	map_header 0, 0, 130, 112, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GaiaRockInside4, .LField_GaiaRockInside4_CELL_PICTURES
	map_block Field_GaiaRockInside4, .LField_GaiaRockInside4_SHAPES
	map_block Field_GaiaRockInside4, .LField_GaiaRockInside4_GRID
	map_block Field_GaiaRockInside4, .LField_GaiaRockInside4_SHAPE_GRID
	map_block Field_GaiaRockInside4, .LField_GaiaRockInside4_ANIMATION
	map_block Field_GaiaRockInside4, .LField_GaiaRockInside4_BLEND
	map_block Field_GaiaRockInside4, .LField_GaiaRockInside4_SCENE
.LField_GaiaRockInside4_CELL_PICTURES:
	.incbin "games/tla/src/map/gaia_rock_inside_4/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_GaiaRockInside4_SHAPES:
	.incbin "games/tla/src/map/gaia_rock_inside_4/shapes.table.lz"
	.balign 4, 0
.LField_GaiaRockInside4_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_4/map.grid.lz"
	.balign 4, 0
.LField_GaiaRockInside4_SHAPE_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_GaiaRockInside4_ANIMATION:
	.incbin "games/tla/src/map/gaia_rock_inside_4/animation.script.lz"
	.balign 4, 0
.LField_GaiaRockInside4_BLEND:
	.incbin "games/tla/src/map/gaia_rock_inside_first_altar/blend.script.lz"
	.balign 4, 0
.LField_GaiaRockInside4_SCENE:
	.incbin "games/tla/src/map/gaia_rock_inside_4/scene.scene.lz"
