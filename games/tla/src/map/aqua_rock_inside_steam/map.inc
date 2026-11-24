	.global Field_AquaRockInsideSteam
Field_AquaRockInsideSteam:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInsideSteam, .LField_AquaRockInsideSteam_CELL_TILES
	map_block Field_AquaRockInsideSteam, .LField_AquaRockInsideSteam_SHAPES
	map_block Field_AquaRockInsideSteam, .LField_AquaRockInsideSteam_GRID
	map_block Field_AquaRockInsideSteam, .LField_AquaRockInsideSteam_SHAPE_GRID
	map_block Field_AquaRockInsideSteam, .LField_AquaRockInsideSteam_ANIMATION
	map_block Field_AquaRockInsideSteam, .LField_AquaRockInsideSteam_BLEND
	map_block Field_AquaRockInsideSteam, .LField_AquaRockInsideSteam_SCENE
.LField_AquaRockInsideSteam_CELL_TILES:
	.incbin "games/tla/src/map/aqua_rock_inside_steam/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AquaRockInsideSteam_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_steam/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInsideSteam_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_steam/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInsideSteam_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_steam/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInsideSteam_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_steam/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInsideSteam_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInsideSteam_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_steam/scene.scene.lz"
	.balign 4, 0
