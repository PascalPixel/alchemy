	.global Field_AquaRockMistSabreChest
Field_AquaRockMistSabreChest:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockMistSabreChest, .LField_AquaRockMistSabreChest_CELL_TILES
	map_block Field_AquaRockMistSabreChest, .LField_AquaRockMistSabreChest_SHAPES
	map_block Field_AquaRockMistSabreChest, .LField_AquaRockMistSabreChest_GRID
	map_block Field_AquaRockMistSabreChest, .LField_AquaRockMistSabreChest_SHAPE_GRID
	map_block Field_AquaRockMistSabreChest, .LField_AquaRockMistSabreChest_ANIMATION
	map_block Field_AquaRockMistSabreChest, .LField_AquaRockMistSabreChest_BLEND
	map_block Field_AquaRockMistSabreChest, .LField_AquaRockMistSabreChest_SCENE
.LField_AquaRockMistSabreChest_CELL_TILES:
	.incbin "games/tla/src/map/aqua_rock_mist_sabre_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AquaRockMistSabreChest_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_mist_sabre_chest/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockMistSabreChest_GRID:
	.incbin "games/tla/src/map/aqua_rock_mist_sabre_chest/map.grid.lz"
	.balign 4, 0
.LField_AquaRockMistSabreChest_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_mist_sabre_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockMistSabreChest_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_mist_sabre_chest/animation.script.lz"
	.balign 4, 0
.LField_AquaRockMistSabreChest_BLEND:
	.incbin "games/tla/src/map/aqua_rock_moai/blend.script.lz"
	.balign 4, 0
.LField_AquaRockMistSabreChest_SCENE:
	.incbin "games/tla/src/map/aqua_rock_mist_sabre_chest/scene.scene.lz"
