	.global Field_AquaRockInsideAquariusStone
Field_AquaRockInsideAquariusStone:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInsideAquariusStone, .LField_AquaRockInsideAquariusStone_CELL_TILES
	map_block Field_AquaRockInsideAquariusStone, .LField_AquaRockInsideAquariusStone_SHAPES
	map_block Field_AquaRockInsideAquariusStone, .LField_AquaRockInsideAquariusStone_GRID
	map_block Field_AquaRockInsideAquariusStone, .LField_AquaRockInsideAquariusStone_SHAPE_GRID
	map_block Field_AquaRockInsideAquariusStone, .LField_AquaRockInsideAquariusStone_ANIMATION
	map_block Field_AquaRockInsideAquariusStone, .LField_AquaRockInsideAquariusStone_BLEND
	map_block Field_AquaRockInsideAquariusStone, .LField_AquaRockInsideAquariusStone_SCENE
.LField_AquaRockInsideAquariusStone_CELL_TILES:
	.incbin "games/tla/src/map/aqua_rock_inside_aquarius_stone/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AquaRockInsideAquariusStone_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_aquarius_stone/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInsideAquariusStone_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_aquarius_stone/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInsideAquariusStone_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_aquarius_stone/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInsideAquariusStone_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_aquarius_stone/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInsideAquariusStone_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInsideAquariusStone_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_aquarius_stone/scene.scene.lz"
	.balign 4, 0
