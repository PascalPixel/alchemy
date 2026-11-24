	.global Field_AquaRockInsideTightrope
Field_AquaRockInsideTightrope:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInsideTightrope, .LField_AquaRockInsideTightrope_CELL_TILES
	map_block Field_AquaRockInsideTightrope, .LField_AquaRockInsideTightrope_SHAPES
	map_block Field_AquaRockInsideTightrope, .LField_AquaRockInsideTightrope_GRID
	map_block Field_AquaRockInsideTightrope, .LField_AquaRockInsideTightrope_SHAPE_GRID
	map_block Field_AquaRockInsideTightrope, .LField_AquaRockInsideTightrope_ANIMATION
	map_block Field_AquaRockInsideTightrope, .LField_AquaRockInsideTightrope_BLEND
	map_block Field_AquaRockInsideTightrope, .LField_AquaRockInsideTightrope_SCENE
.LField_AquaRockInsideTightrope_CELL_TILES:
	.incbin "games/tla/src/map/aqua_rock_inside_tightrope/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AquaRockInsideTightrope_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_tightrope/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInsideTightrope_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_tightrope/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInsideTightrope_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_tightrope/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInsideTightrope_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInsideTightrope_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInsideTightrope_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_tightrope/scene.scene.lz"
