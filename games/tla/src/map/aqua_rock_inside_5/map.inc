	.global Field_AquaRockInside5
Field_AquaRockInside5:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInside5, .LField_AquaRockInside5_CELL_PICTURES
	map_block Field_AquaRockInside5, .LField_AquaRockInside5_SHAPES
	map_block Field_AquaRockInside5, .LField_AquaRockInside5_GRID
	map_block Field_AquaRockInside5, .LField_AquaRockInside5_SHAPE_GRID
	map_block Field_AquaRockInside5, .LField_AquaRockInside5_ANIMATION
	map_block Field_AquaRockInside5, .LField_AquaRockInside5_BLEND
	map_block Field_AquaRockInside5, .LField_AquaRockInside5_SCENE
.LField_AquaRockInside5_CELL_PICTURES:
	.incbin "games/tla/src/map/aqua_rock_inside_5/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AquaRockInside5_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_5/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInside5_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_5/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInside5_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_5/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInside5_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInside5_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInside5_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_5/scene.scene.lz"
