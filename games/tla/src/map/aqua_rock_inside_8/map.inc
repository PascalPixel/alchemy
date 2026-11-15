	.global Field_AquaRockInside8
Field_AquaRockInside8:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInside8, .LField_AquaRockInside8_CELL_PICTURES
	map_block Field_AquaRockInside8, .LField_AquaRockInside8_SHAPES
	map_block Field_AquaRockInside8, .LField_AquaRockInside8_GRID
	map_block Field_AquaRockInside8, .LField_AquaRockInside8_SHAPE_GRID
	map_block Field_AquaRockInside8, .LField_AquaRockInside8_ANIMATION
	map_block Field_AquaRockInside8, .LField_AquaRockInside8_BLEND
	map_block Field_AquaRockInside8, .LField_AquaRockInside8_SCENE
.LField_AquaRockInside8_CELL_PICTURES:
	.incbin "games/tla/src/map/aqua_rock_inside_8/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AquaRockInside8_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_8/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInside8_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_8/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInside8_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_8/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInside8_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInside8_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInside8_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_8/scene.scene.lz"
